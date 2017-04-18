#ifndef __SIMUCOPTER_COPTER_H_
#define __SIMUCOPTER_COPTER_H_

// TODO: Fix this somehow - this is horrible...
#define private public

#include "Copter.h"
#include "simucopter-funcs.h"

inline double copter_get_accel_x() { return copter.ins.get_accel(0).x; }
inline double copter_get_accel_y() { return copter.ins.get_accel(0).y; }
inline double copter_get_accel_z() { return copter.ins.get_accel(0).z; }

inline double copter_get_gyro_x() { return copter.ins.get_gyro(0).x; }
inline double copter_get_gyro_y() { return copter.ins.get_gyro(0).y; }
inline double copter_get_gyro_z() { return copter.ins.get_gyro(0).z; }

inline double copter_get_state_yaw() { return copter.ahrs.yaw; }
inline double copter_get_state_roll() { return copter.ahrs.roll; }
inline double copter_get_state_pitch() { return copter.ahrs.pitch; }

inline double copter_get_desired_yaw() { return copter.get_pilot_desired_yaw_rate(copter.channel_yaw->control_in); }
inline double copter_get_desired_roll() { return -1.0; /* FIXME */ }
inline double copter_get_desired_pitch() { return -1.0; /* FIXME */ }
inline double copter_get_desired_throttle() { return copter.get_pilot_desired_throttle(copter.channel_throttle->control_in); }

inline void copter_set_rate_target_yaw(double yaw) { copter.attitude_control->rate_bf_yaw_target(yaw); }
inline void copter_set_rate_target_pitch(double pitch) { copter.attitude_control->rate_bf_pitch_target(pitch); }
inline void copter_set_rate_target_roll(double roll) { copter.attitude_control->rate_bf_roll_target(roll); }

// see ardupilot/libraries/AP_Motors/AP_Motors_Class.h
inline void copter_motors_set_roll(double roll) { copter.motors->set_roll(roll); }
inline void copter_motors_set_pitch(double pitch) { copter.motors->set_pitch(pitch); }
inline void copter_motors_set_yaw(double yaw) { copter.motors->set_yaw(yaw); }
inline void copter_motors_set_throttle(double throttle) { copter.motors->set_throttle(throttle); }

inline void copter_gcs_send_text(MAV_SEVERITY severity, const char* str) { copter.gcs_send_text(severity, str); }

#endif
