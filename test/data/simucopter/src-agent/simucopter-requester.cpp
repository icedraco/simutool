#include "simucopter-requester.h"

#define SITL_REQ_DOUBLE(id) bridge_req_double(sock_req_sitl, id);
#define AC_REQ_DOUBLE(id) bridge_req_double(sock_req_arducopter, id);
#define AC_REQ_SET_DOUBLE(id, param) bridge_req_set_double(sock_req_arducopter, id, param);

void* sock_req_arducopter = NULL;
void* sock_req_sitl = NULL;


/******************************************************************************\
 * Primary Functions **********************************************************
\******************************************************************************/

void requester_init() {
    bridge_init();

    sock_req_arducopter = bridge_req_socket(ADDR_ARDUCOPTER);
    sock_req_sitl = bridge_req_socket(ADDR_SITL);

    assert(sock_req_arducopter);
    assert(sock_req_sitl);
}

void requester_stop() {
    if (sock_req_sitl)
        bridge_close_socket(sock_req_sitl);

    if (sock_req_arducopter)
        bridge_close_socket(sock_req_arducopter);

    bridge_stop();
}


/******************************************************************************\
 * STUBS **********************************************************************
\******************************************************************************/

double copter_get_accel_x() {
    return AC_REQ_DOUBLE(MSG_GET_ACCEL_X);
}

double copter_get_accel_y() {
    return AC_REQ_DOUBLE(MSG_GET_ACCEL_Y);
}

double copter_get_accel_z() {
    return AC_REQ_DOUBLE(MSG_GET_ACCEL_Z);
}

double copter_get_gyro_x() {
    return AC_REQ_DOUBLE(MSG_GET_GYRO_X);
}

double copter_get_gyro_y() {
    return AC_REQ_DOUBLE(MSG_GET_GYRO_Y);
}

double copter_get_gyro_z() {
    return AC_REQ_DOUBLE(MSG_GET_GYRO_Z);
}

double copter_get_state_yaw() {
    return AC_REQ_DOUBLE(MSG_GET_STATE_YAW);
}

double copter_get_state_roll() {
    return AC_REQ_DOUBLE(MSG_GET_STATE_ROLL);
}

double copter_get_state_pitch() {
    return AC_REQ_DOUBLE(MSG_GET_STATE_PITCH);
}

double copter_get_desired_yaw() {
    return AC_REQ_DOUBLE(MSG_GET_DESIRED_YAW);
}

double copter_get_desired_roll() {
    return AC_REQ_DOUBLE(MSG_GET_DESIRED_ROLL);
}

double copter_get_desired_pitch() {
    return AC_REQ_DOUBLE(MSG_GET_DESIRED_PITCH);
}

double copter_get_desired_throttle() {
    return AC_REQ_DOUBLE(MSG_GET_DESIRED_THROTTLE);
}

void copter_set_rate_target_yaw(double yaw) {
    AC_REQ_SET_DOUBLE(MSG_SET_RATE_TARGET_YAW, yaw);
}

void copter_set_rate_target_pitch(double pitch) {
    AC_REQ_SET_DOUBLE(MSG_SET_RATE_TARGET_PITCH, pitch);
}

void copter_set_rate_target_roll(double roll) {
    AC_REQ_SET_DOUBLE(MSG_SET_RATE_TARGET_ROLL, roll);
}

// see ardupilot/libraries/AP_Motors/AP_Motors_Class.h
void copter_motors_set_roll(double roll) {
    AC_REQ_SET_DOUBLE(MSG_SET_MOTORS_ROLL, roll);
}

void copter_motors_set_pitch(double pitch) {
    AC_REQ_SET_DOUBLE(MSG_SET_MOTORS_PITCH, pitch);
}

void copter_motors_set_yaw(double yaw) {
    AC_REQ_SET_DOUBLE(MSG_SET_MOTORS_YAW, yaw);
}

void copter_motors_set_throttle(double throttle) {
    AC_REQ_SET_DOUBLE(MSG_SET_MOTORS_THROTTLE, throttle);
}

void copter_gcs_send_text(int severity, const char* str) {
    assert(0); // TODO: not yet implemented
}


void sitl_whatever() {
    return AC_REQ_DOUBLE(MSG_SITL_WHATEVER);
}
