import unittest
import os

import tools.tracer as tracer


###############################################################################
# --- PATHS ----------------------------------------------------------------- #
###############################################################################

# this dir contains all the simucopter-related sources for all sides
SIMUCOPTER_DIR = os.path.join('data', 'simucopter')

# ardupilot-related code: this should be the primary location in case of file ambiguity!
SRC_ARDUPILOT_DIR = os.path.join(SIMUCOPTER_DIR, 'src-ardupilot')

# agent-related addon code: this should be a secondary location in case of file ambiguity!
SRC_SIMULINK_DIR = os.path.join(SIMUCOPTER_DIR, 'src-agent')

# this dir contains all agent-provided code: this is the tertiary location
AGENT_DIR = os.path.join('data', 'simulink_code')


###############################################################################
# --- FILES ----------------------------------------------------------------- #
###############################################################################

# initial CPP files we require to be present in the ArduCopter code tree
INIT_FILES = ['TestFlightMode.cpp', 'TestFlightMode_data.cpp', 'simucopter-ardupilot.cpp']

# this file will be included, but it's part of Arducopter - assume it already exists
EXCLUDED_FILES = ['Copter.h', 'bridge.h']


class TracerBasic(unittest.TestCase):

    def setUp(self):
        self.assertTrue(os.path.isdir(AGENT_DIR), os.path.join(os.getcwd(), AGENT_DIR))
        self.assertTrue(os.path.isdir(SIMUCOPTER_DIR), os.path.join(os.getcwd(), SIMUCOPTER_DIR))
        self.assertTrue(os.path.isdir(SRC_ARDUPILOT_DIR), os.path.join(os.getcwd(), SRC_ARDUPILOT_DIR))
        self.assertTrue(os.path.isdir(SRC_SIMULINK_DIR), os.path.join(os.getcwd(), SRC_SIMULINK_DIR))

        for f in INIT_FILES:
            option1 = os.path.join(SRC_ARDUPILOT_DIR, f)
            option2 = os.path.join(SRC_SIMULINK_DIR, f)
            option3 = os.path.join(AGENT_DIR, f)
            self.assertTrue(any([os.path.isfile(option) for option in [option1, option2, option3]]), f)

    def test_all_requirements_met(self):
        needed_files = [
            'TestFlightMode.cpp',
            'TestFlightMode_data.cpp',
            'TestFlightMode.h',
            'TestFlightMode_types.h',
            'TestFlightMode_private.h',
            'TestFlightMode_dt.h',

            'multiword_types.h',
            'rtwtypes.h',
            'rtw_extmode.h',
            'sysran_types.h',
            'dt_info.h',
            'ext_work.h',
            'ext_types.h',

            # should be included with Simulink - we don't want these
            # 'simucopter-agent.cpp',
            # 'simucopter-requester.h',
            # 'simucopter-requester.cpp',

            # should be included from ardupilot - missing from Simulink
            'simucopter.h',
            'simucopter-funcs.h',
            'simucopter-ardupilot.h',
            'simucopter-ardupilot.cpp',
            'simucopter-copter.h',
        ]

        t = tracer.Tracer(SRC_ARDUPILOT_DIR)
        t.addpath(SRC_SIMULINK_DIR)
        t.addpath(AGENT_DIR)
        t.exclude(EXCLUDED_FILES)

        [t.load(f) for f in INIT_FILES]

        self.assertEqual(set(needed_files), set(t.items_processed))

        # ensure ambiguous files were taken from the correct place
        ambiguous_arducopter = [os.path.join(SRC_ARDUPILOT_DIR, f) for f in [
            'simucopter.h',
            'simucopter-funcs.h',
        ]]

        for f in ambiguous_arducopter:
            self.assertTrue(f in t.files, f)


if __name__ == '__main__':
    unittest.main()
