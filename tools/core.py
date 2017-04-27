import os
import logging


G_CWD = os.getcwd()
G_HOME = os.environ['HOME'] if 'HOME' in os.environ else '.'
G_QUICK_ARDUPILOT_SUPERDIRS = [G_CWD, G_HOME, '..']


log = logging.getLogger(__name__)


class FlightMode(object):
    """
    Represents a SimuCopter Flight Mode, and contains related to it data
    """

    def __init__(self, name, simulink_build_path):
        """
        :param str name: flight mode name (e.g., "TestFlightMode" for "TestFlightMode_ert_rtw")
        :param str simulink_build_path: build path configured in SimuLink
        """
        self.name = name
        self.simulink_build_path = simulink_build_path

    def __str__(self):
        return self.name

    @property
    def path(self):
        """
        :return str: path to the flight mode code delivered by Simulink
        """
        return os.path.abspath(os.path.join(self.simulink_build_path, "{}_ert_rtw".format(self.name)))

    @property
    def path_exists(self):
        """
        :return bool: True if flight mode path exists; False if it doesn't
        """
        return os.path.isdir(self.path)

    @property
    def init_cpp_files(self):
        """
        :return list[str]: initial C++ files from this flight mode to link to ArduCopter
        """
        return [
            self.name + ".cpp",
            self.name + "_data.cpp",
            'simucopter-ardupilot.cpp',
        ]


def is_ardupilot_dir(root, dirs, _):
    has_all_dirs = {'ArduCopter', 'ArduPlane', 'Tools'}.issubset(dirs)
    has_all_files = all([os.path.isfile(os.path.join(root, 'ArduCopter', f)) for f in {'Copter.cpp', 'Copter.h'}])
    return has_all_dirs and has_all_files


def detect_ardupilot_dir_quick():
    log.debug("ArduPilot path detection (QUICK) is running...")
    for dir in G_QUICK_ARDUPILOT_SUPERDIRS:
        log.debug("  > Checking {}".format(dir))
        root, dirs, files = next(os.walk(dir))

        for d in [os.path.join(root, d) for d in dirs]:
            log.debug("      >> {}...".format(d))
            if is_ardupilot_dir(*next(os.walk(d))):
                log.debug("      !! FOUND!")
                return d

    log.debug("  X Giving up - not found")
    return None  # couldn't find any
