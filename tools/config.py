import os
import logging

from ConfigParser import ConfigParser
from core import detect_ardupilot_dir_quick


G_CONFIG = None
G_CONFIG_FILENAME = 'config.ini'
G_AUTOSAVE = False


class Config(object):
    DEFAULT_NUM_JOBS = 4

    def __init__(self, filename):
        """
        :param str filename: filename to load configuration from
        """
        self.log = logging.getLogger('.'.join([__name__, self.__class__.__name__]))
        self.filename = filename
        self.conf = ConfigParser()

        init_success = self.conf.read(filename) != []
        status = "CREATED" if not init_success else "LOADED"
        self.log.info("Initialized from {} [{}]".format(self.filename, status))
        _ = self.ardupilot_dir  # initial detection

    @property
    def num_jobs(self):
        if not self.conf.has_option('general', 'num_jobs'):
            self.conf.set('general', 'num_jobs', str(self.DEFAULT_NUM_JOBS))
        return self.conf.getint('general', 'num_jobs')

    @num_jobs.setter
    def num_jobs(self, value):
        self.conf.set('general', 'num_jobs', int(value))

    @property
    def sim_vehicle(self):
        """
        :return: path to the sim_vehicle.py script
        :rtype: str
        """
        return os.path.join(self.ardupilot_dir, 'Tools', 'autotest', 'sim_vehicle.py')

    @property
    def arducopter_dir(self):
        """
        :return: path to the ArduCopter directory within ArduPilot
        :rtype: str
        """
        return os.path.join(self.ardupilot_dir, 'ArduCopter')

    @property
    def ardupilot_dir(self):
        """
        :return: path to the ArduPilot directory
        :rtype: str
        """
        self.__ensure_section('general')
        if not self.conf.has_option('general', 'ardupilot_dir'):
            self.log.info("ArduPilot dir unknown - locating...")
            d = detect_ardupilot_dir_quick()
            if not d:
                raise RuntimeError("Cannot find ArduPilot path")

            self.conf.set('general', 'ardupilot_dir', d)

            if G_AUTOSAVE:
                self.save()

        return self.conf.get('general', 'ardupilot_dir')

    def save(self):
        """
        Save current configuration to disk
        """
        self.log.info("Saving configuration to {}".format(self.filename))
        self.conf.write(open(self.filename, 'w'))

    def __ensure_section(self, sec):
        if not self.conf.has_section(sec):
            self.conf.add_section(sec)
        assert self.conf.has_section(sec)


def get_master_config():
    """
    :return: master configuration object
    :rtype Config
    """
    global G_CONFIG
    if G_CONFIG is None:
        G_CONFIG = Config(G_CONFIG_FILENAME)
    return G_CONFIG
