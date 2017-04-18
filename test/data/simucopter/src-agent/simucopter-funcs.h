#ifndef __SIMULINK_FUNCS_H_
#define __SIMULINK_FUNCS_H_

double copter_get_accel_x();
double copter_get_accel_y();
double copter_get_accel_z();

double copter_get_gyro_x();
double copter_get_gyro_y();
double copter_get_gyro_z();

double copter_get_state_yaw();
double copter_get_state_roll();
double copter_get_state_pitch();

double copter_get_desired_yaw();
double copter_get_desired_roll();
double copter_get_desired_pitch();
double copter_get_desired_throttle();

void copter_set_rate_target_yaw(double yaw);
void copter_set_rate_target_pitch(double pitch);
void copter_set_rate_target_roll(double roll);

// see ardupilot/libraries/AP_Motors/AP_Motors_Class.h
void copter_motors_set_roll(double roll);
void copter_motors_set_pitch(double pitch);
void copter_motors_set_yaw(double yaw);
void copter_motors_set_throttle(double throttle);

void copter_gcs_send_text(int severity, const char* str);

#endif
