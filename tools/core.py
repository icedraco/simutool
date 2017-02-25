import os
import logging


G_CWD = os.getcwd()
G_HOME = os.environ['HOME'] if 'HOME' in os.environ else '.'
G_QUICK_ARDUPILOT_SUPERDIRS = [G_CWD, G_HOME, '..']


log = logging.getLogger(__name__)


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
