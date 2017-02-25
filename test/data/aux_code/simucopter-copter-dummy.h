#ifndef __SIMUCOPTER_COPTER_H_
#define __SIMUCOPTER_COPTER_H_

#include "simucopter-funcs.h"

inline double copter_get_accel_x() { return -1.0; }
inline double copter_get_accel_y() { return -1.0; }
inline double copter_get_accel_z() { return -1.0; }

inline double copter_get_gyro_x() { return -1.0; }
inline double copter_get_gyro_y() { return -1.0; }
inline double copter_get_gyro_z() { return -1.0; }

inline double copter_get_state_yaw()   { return -1.0; }
inline double copter_get_state_roll()  { return -1.0; }
inline double copter_get_state_pitch() { return -1.0; }

inline double copter_get_desired_yaw()      { return -1.0; }
inline double copter_get_desired_roll()     { return -1.0; }
inline double copter_get_desired_pitch()    { return -1.0; }
inline double copter_get_desired_throttle() { return -1.0; }

inline void copter_set_rate_target_yaw(double yaw)     {}
inline void copter_set_rate_target_pitch(double pitch) {}
inline void copter_set_rate_target_roll(double roll)   {}

// see ardupilot/libraries/AP_Motors/AP_Motors_Class.h
inline void copter_motors_set_roll(double roll)         {}
inline void copter_motors_set_pitch(double pitch)       {}
inline void copter_motors_set_yaw(double yaw)           {}
inline void copter_motors_set_throttle(double throttle) {}

inline void copter_gcs_send_text(int severity, const char* str) {}

#endif
