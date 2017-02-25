import unittest
import os

import tools.tracer as tracer


CODE_DIR = os.path.join('data', 'simulink_code')
AUX_CODE_DIR = os.path.join('data', 'aux_code')

INIT_FILES = ['TestFlightMode.cpp', 'TestFlightMode_data.cpp', 'simucopter-agent.cpp']
EXCLUDED_FILES = ['Copter.h']


class TracerBasic(unittest.TestCase):

    # def setUp(self):
    #     self.tracer = tracer.trace(self.ROOT_FILE)

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

            'simucopter.h',
            'simucopter-requester.h',
            'simucopter-protocol.h',
            'simucopter-transport.h',
            'simucopter-funcs.h',
            'simucopter-agent.cpp',

            'simucopter-copter.h',  # this one's missing from Simulink package
            'simucopter-server.h',  # this one's missing from Simulink package
        ]

        t = tracer.Tracer(CODE_DIR)
        t.addpath(AUX_CODE_DIR)
        t.exclude(EXCLUDED_FILES)

        [t.load(f) for f in INIT_FILES]

        self.assertEqual(set(needed_files), set(t.items_processed))


if __name__ == '__main__':
    unittest.main()
