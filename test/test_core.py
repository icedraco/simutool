import unittest

from tools.core import FlightMode


class CoreFlightModeTestCase(unittest.TestCase):

    def setUp(self):
        self.fm = FlightMode("Test", "/tmp/simulink_build_path")

    def test_str_resolves_to_flight_mode_name(self):
        self.assertEqual(self.fm.name, str(self.fm))

    def test_flight_mode_build_path_is_correct(self):
        import os
        expected_path_suffix = os.path.join("tmp", "simulink_build_path", "{}_ert_rtw".format(self.fm.name))
        self.assertTrue(self.fm.path.endswith(expected_path_suffix))

    def test_flight_mode_cpp_files_included_in_init_cpp_files(self):
        expected_files = [
            "{}.cpp".format(self.fm.name),
            "{}_data.cpp".format(self.fm.name),
        ]

        self.assertTrue(all([f in self.fm.init_cpp_files for f in expected_files]))

    def test_simucopter_source_included_in_init_cpp_files(self):
        expected_files = [
            "simucopter-server.cpp",
            "simucopter-ardupilot.cpp",
        ]

        self.assertTrue(all([f in self.fm.init_cpp_files for f in expected_files]))


if __name__ == '__main__':
    unittest.main()
