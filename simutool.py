#!/usr/bin/python2.7

import sys
import tools

from tools.core import *
from tools.config import get_master_config

log = logging.getLogger(__name__)

G_HOME = '/home/pi'

# simucopter code installation path and its source paths
G_SIMUCOPTER_PATH = os.path.join(G_HOME, 'simucopter')
G_SRC_AGENT = os.path.join(G_SIMUCOPTER_PATH, 'src-agent')
G_SRC_ARDUPILOT = os.path.join(G_SIMUCOPTER_PATH, 'src-ardupilot')
G_SRC_SITL = os.path.join(G_SIMUCOPTER_PATH, 'src-sitl')
G_SRC_BRIDGE = os.path.join(G_SIMUCOPTER_PATH, 'bridge')

# log path
G_LOG_PATH = os.path.join(G_SIMUCOPTER_PATH, 'log')

# name of the configuration file
G_CONFIG_FILENAME = 'simucopter.ini'

# where should the configuration file be located?
G_CONFIG_PATH = os.path.join(G_HOME, G_CONFIG_FILENAME)

# Simulink build path (where flight mode code is deployed)
G_SIMULINK_BUILD_PATH = G_SIMUCOPTER_PATH

# files excluded from linking into ArduPilot source
G_EXCLUDED_FILES = [
    'Copter.h',  # provided by ArduCopter itself
    'bridge.h',  # provided by libraries/bridge/
]


def link_fm_file(fm_path, link_path):
    """
    Link flight mode file from `fm_path` to `link_path`
    
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
    Link an entire set of flight mode files to ArduCopter
    
    :param FlightMode fm: flight mode to link
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

    log.info("Removing existing links from ArduCopter directory...")
    from glob import glob
    all_links = filter(os.path.islink, glob(os.path.join(arducopter_dir, '*')))
    for l in all_links:
        os.unlink(l)
        log.info("  > {}".format(l))

    log.info("Expanding list with included files...")
    t = tools.tracer.Tracer(G_SRC_ARDUPILOT)
    t.addpath(G_SRC_AGENT)
    t.addpath(fm.path)
    t.exclude(G_EXCLUDED_FILES)

    [t.load(fname) for fname in fm.init_cpp_files]
    included_files = t.files
    log.info("")

    log.info("Included files:")
    [log.info("  >> {}".format(fname)) for fname in included_files]
    log.info("")

    # obtain full paths
    included_files_full = [os.path.join(fm.path, fname) for fname in included_files]

    # link all included files we've discovered
    for fm_file in included_files_full:
        link_file = os.path.join(arducopter_dir, os.path.basename(fm_file))
        link_fm_file(fm_file, link_file)

    # link current flight mode .h file as current-flight-mode.h
    prev_dir = os.getcwd()
    os.chdir(arducopter_dir)
    os.symlink("{}.h".format(fm.name), "current-flight-mode.h")
    os.chdir(prev_dir)
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

    exit_code = do_build()
    if exit_code != 0:
        log.error("Exit code {} -> aborted".format(exit_code))
        return exit_code

    return do_run()


def do_build():
    log.info("Building project...")
    conf = get_master_config()
    os.chdir(conf.ardupilot_dir)
    log.info("CWD: {}".format(os.getcwd()))
    exit_code = do_command(['./waf', 'configure', '--board', 'sitl'])
    if exit_code != 0:
        log.error("Exit code {} -> aborted".format(exit_code))
        return exit_code

    exit_code = do_command(['./waf', 'build', '--target', 'bin/arducopter', '-j', conf.num_jobs])
    if exit_code != 0:
        log.error("Exit code {} -> aborted".format(exit_code))
        return exit_code

    return 0


def do_run():
    log.info("Trying to run project...")
    os.chdir(get_master_config().ardupilot_dir)
    log.info("CWD: {}".format(os.getcwd()))
    return do_command(['./build/sitl/bin/arducopter',
                       '-S',
                       '-I0',
                       '--home', '-35.363261,149.165230,584,353',
                       '--model', '+',
                       '--speedup', '1',
                       '--defaults', './Tools/autotest/default_params/copter.parm'])


def do_kill():
    log.info("Locating and killing processes...")
    # ps -ef | grep your_process_name | grep -v grep | awk '{print $2}' | xargs kill
    os.system("ps -ef | grep TestFlightMode | grep -v grep | awk '{print $2}' | xargs kill")
    os.system("ps -ef | grep copter | grep -v grep | awk '{print $2}' | xargs kill")


def print_syntax(argv):
    print "Syntax: {} deploy <flight mode hint>".format(argv[0])
    print "        {} link <flight mode path>".format(argv[0])
    print "        {} build".format(argv[0])
    print "        {} run".format(argv[0])
    print


def main(argv):
    if len(argv) == 1:
        print_syntax(argv)
        return 1

    cmd_map = {
        'link': lambda: do_link_flight_mode(argv[2]),
        'deploy': lambda: do_deploy(argv),
        'build': lambda: do_build(),
        'run': lambda: do_run(),
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
