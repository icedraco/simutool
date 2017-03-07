#!/usr/bin/python2.7

import sys
import tools

from tools.core import *
from tools.config import get_master_config

log = logging.getLogger(__name__)

G_HOME = '/home/pi'

# simucopter code installation path
G_SIMUCOPTER_PATH = os.path.join(G_HOME, 'simucopter')

# log path
G_LOG_PATH = os.path.join(G_SIMUCOPTER_PATH, 'log')

# name of the configuration file
G_CONFIG_FILENAME = 'simucopter.ini'

# where should the configuration file be located?
G_CONFIG_PATH = os.path.join(G_HOME, G_CONFIG_FILENAME)

# Simulink build path (where flight mode code is deployed)
G_SIMULINK_BUILD_PATH = G_SIMUCOPTER_PATH

# auxiliary code that may be required, but not available from Simulink deployment package
G_AUX_CODE_DIR = os.path.join(G_HOME, 'simucopter', 'bridge')

# files excluded from linking into ArduPilot source
G_EXCLUDED_FILES = ['Copter.h']


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


def do_link_flight_mode(fm):
    """
    :param FlightMode fm: flight mode to link to ArduCopter
    :return int: return code
    """

    assert os.path.isdir(fm.path) or os.path.islink(fm.path)
    assert fm.path.endswith('_ert_rtw')

    arducopter_dir = get_master_config().arducopter_dir

    log.info("")
    log.info("Attempting to link flight mode...")
    log.info("  > Flight Mode: {}".format(fm))
    log.info("  > ArduCopter:  {}".format(arducopter_dir))
    log.info("  > Code:        {}".format(fm.path))
    [log.info("      >> {}".format(fname)) for fname in fm.init_cpp_files]
    log.info("")

    log.info("Expanding list with included files...")
    t = tools.tracer.Tracer(fm.path)
    t.addpath(G_AUX_CODE_DIR)
    t.exclude(G_EXCLUDED_FILES)

    [t.load(fname) for fname in fm.init_cpp_files]
    included_files = t.files
    log.info("")

    log.info("Included files:")
    [log.info("  >> {}".format(fname)) for fname in included_files]
    log.info("")

    # obtain full paths
    included_files_full = [os.path.join(fm.path, fname) for fname in included_files]

    for fm_file in included_files_full:
        link_file = os.path.join(arducopter_dir, os.path.basename(fm_file))
        link_fm_file(fm_file, link_file)

    return 0


def do_command(cmd):
    """
    :param list[str] cmd: command to execute as a list of argument strings
    :return int: exit code
    """
    import subprocess
    cmd = map(str, cmd)  # cleanup
    log.info(">> {}".format(' '.join(cmd)))
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    while True:
        line = proc.stdout.readline()
        if not line:
            break

        log.info(line.strip())

    return proc.wait()  # exit code


def do_deploy(argv):
    fm_hint = argv[2]
    fm_name = os.path.basename(fm_hint).split('.')[0]

    fm = FlightMode(fm_name, G_SIMULINK_BUILD_PATH)

    log.info("--- DEPLOY: {} -----------------------------".format(fm_name))
    log.info(" > Path: {} [{}]".format(fm.path, "OK" if fm.path_exists else "BAD"))
    log.info("")
    if not os.path.isdir(fm.path):
        log.error("Proposed fm_path is not a directory!!")
        return 1

    log.info("Running link operation against flight mode path...")
    do_link_flight_mode(fm)

    conf = get_master_config()

    log.info("Building project...")
    os.chdir(conf.ardupilot_dir)
    exit_code = do_command(['./waf', 'configure', '--board', 'sitl'])
    if exit_code != 0:
        log.error("Exit code {} -> aborted".format(exit_code))
        return exit_code

    exit_code = do_command(['./waf', 'build', '--target', 'bin/arducopter', '-j', conf.num_jobs])
    if exit_code != 0:
        log.error("Exit code {} -> aborted".format(exit_code))
        return exit_code

    log.info("Trying to run project...")
    return do_command(['./build/sitl/bin/arducopter',
                       '-S',
                       '-I0',
                       '--home', '-35.363261,149.165230,584,353',
                       '--model', '+',
                       '--speedup', '1',
                       '--defaults', './Tools/autotest/default_params/copter.parm'])


def print_syntax(argv):
    print "Syntax: {} deploy <flight mode hint>".format(argv[0])
    print "        {} link <flight mode path>".format(argv[0])
    print


def main(argv):
    if len(argv) == 1:
        print_syntax(argv)
        return 1

    cmd_map = {
        'link': lambda: do_link_flight_mode(argv[2]),
        'deploy': lambda: do_deploy(argv),
    }

    cmd = argv[1]

    if cmd.lower() not in cmd_map:
        print_syntax(argv)
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
    if not os.path.isdir(G_LOG_PATH):
        os.makedirs(G_LOG_PATH)
        assert os.path.isdir(G_LOG_PATH)

    logging.getLogger().setLevel(logging.DEBUG)
    logging.getLogger().addHandler(logging.StreamHandler(sys.stdout))
    logging.getLogger().addHandler(logging.FileHandler(os.path.join(G_LOG_PATH, 'runtime.log'), 'w'))

    raise SystemExit(main(sys.argv))
