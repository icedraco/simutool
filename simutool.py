#!/usr/bin/python2.7

import os
import sys
import tools
import logging

from tools.config import get_master_config


log = logging.getLogger(__name__)


G_HOME = '/home/pi'

# name of the configuration file
G_CONFIG_FILENAME = 'simucopter.ini'

# where should the configuration file be located?
G_CONFIG_PATH = os.path.join(G_HOME, G_CONFIG_FILENAME)

# Simulink build path (where flight mode code is deployed)
G_SIMULINK_BUILD_PATH = os.path.join(G_HOME, 'simucopter')

# auxiliary code that may be required, but not available from Simulink deployment package
G_AUX_CODE_DIR = os.path.join(G_HOME, 'simucopter', 'bridge')

# files excluded from linking into ArduPilot source
G_EXCLUDED_FILES = ['Copter.h']


def get_init_files(fm_name):
    """
    :param str fm_name: flight mode name
    :return: list of required C++ files for the project
    :rtype: list[str]
    """
    return [
        fm_name + ".cpp",
        fm_name + "_data.cpp",
        'simucopter-agent.cpp'
    ]


def link_fm_file(fm_path, link_path):
    """
    :param (str) fm_path: path to original file
    :param (str) link_path: path to target symbolic link file
    :return: False if target link file exists, and is not a symlink to begin with
    :rtype: bool
    """
    log.debug("--- LINK -----------------------------------")
    log.debug(" > Original: {}".format(fm_path))
    log.debug(" > Link:     {}".format(link_path))
    log.debug("")

    if os.path.isfile(link_path) and not os.path.islink(link_path):
        log.warn(" X WARNING: Destination file exist and is NOT a symlink! Skipped!")
        log.debug("")
        return False

    if os.path.islink(link_path):
        log.debug(" ! Removing existing symlink from target_path...")
        os.unlink(link_path)

    os.symlink(fm_path, link_path)
    log.debug(" + Link successful")
    log.debug("")
    return True


def do_link_flight_mode(fm_path):
    """
    :param str fm_path: path to the flight mode directory (including flight mode dir name itself)
    :return: return code
    :rtype: int
    """
    fm_path = os.path.abspath(fm_path)

    assert os.path.isdir(fm_path) or os.path.islink(fm_path)
    assert fm_path.endswith('_ert_rtw')

    config = get_master_config()

    fm_name = os.path.basename(fm_path).split('_ert_rtw')[-2]
    init_files = get_init_files(fm_name)
    arducopter_dir = config.arducopter_dir

    log.info("")
    log.info("Attempting to link flight mode...")
    log.info("  > Flight Mode: {}".format(fm_name))
    log.info("  > ArduCopter:  {}".format(arducopter_dir))
    log.info("  > Code:        {}".format(fm_path))
    [log.info("      >> {}".format(fname)) for fname in init_files]
    log.info("")

    log.info("Expanding list with included files...")
    t = tools.tracer.Tracer(fm_path)
    t.addpath(G_AUX_CODE_DIR)
    t.exclude(G_EXCLUDED_FILES)

    [t.load(fname) for fname in init_files]
    included_files = t.files
    log.info("")

    log.info("Included files:")
    [log.info("  >> {}".format(fname)) for fname in included_files]
    log.info("")

    # obtain full paths
    included_files_full = [os.path.join(os.path.abspath(fm_path), fname) for fname in included_files]

    for fm_file in included_files_full:
        link_file = os.path.join(arducopter_dir, os.path.basename(fm_file))
        link_fm_file(fm_file, link_file)

    return 0


def do_sim_vehicle(postfix=""):
    """
    :param str postfix: additional parameters to add to the end of the sim_vehicle command
    :return: return code
    :rtype: int
    """
    config = get_master_config()
    old_cwd = os.getcwd()
    os.chdir(config.arducopter_dir)

    import subprocess
    proc = subprocess.Popen((config.sim_vehicle, postfix), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    process_output, _ = proc.communicate("")
    log.debug(process_output)
    exit_code = proc.wait()
    log.warning("WARNING: sim_vehicle process exited with code {}".format(exit_code))

    os.chdir(old_cwd)
    return exit_code


def do_deploy(argv):
    fm_hint = argv[2]
    fm_name = os.path.basename(fm_hint).split('.')[0]
    fm_path = os.path.join(G_SIMULINK_BUILD_PATH, "{}_ert_rtw".format(fm_name))
    fm_path_status = "OK" if os.path.isdir(fm_path) else "BAD"

    log.info("--- DEPLOY: {} -----------------------------".format(fm_name))
    log.info(" > Path: {} [{}]".format(fm_path, fm_path_status))
    log.info("")
    if not os.path.isdir(fm_path):
        log.error("Proposed fm_path is not a directory!!")
        return 1

    log.info("Running link operation against flight mode path...")
    do_link_flight_mode(fm_path)

    log.info("Trying to run project...")
    do_sim_vehicle("-j 4")
    return 0


def do_print_syntax(argv):
    print "Syntax: {} deploy <flight mode hint>".format(argv[0])
    print "        {} link <flight mode path>".format(argv[0])
    print "        {} run".format(argv[0])
    print


def main(argv):
    if len(argv) == 1:
        do_print_syntax(argv)
        return 1

    cmd_map = {
        'link': lambda: do_link_flight_mode(argv[2]),
        'run': lambda: do_sim_vehicle("-j 4"),
        'deploy': lambda: do_deploy(argv),
    }

    cmd = argv[1]

    if cmd.lower() not in cmd_map:
        do_print_syntax(argv)
        print
        print "(invalid command '{}')".format(cmd)
        print
        return 2

    log.info("CWD:           {}".format(os.getcwd()))
    log.info("ArduPilot dir: {}".format(get_master_config().ardupilot_dir))
    log.info("Command:       {}".format(cmd))

    try:
        return cmd_map[cmd.lower()]()
    except Exception as ex:
        log.exception("Exception: ".format(ex))
        return 3


if __name__ == '__main__':
    logging.getLogger().setLevel(logging.DEBUG)
    logging.getLogger().addHandler(logging.StreamHandler(sys.stdout))
    logging.getLogger().addHandler(logging.FileHandler('runtime.log', 'w'))
    raise SystemExit(main(sys.argv))
