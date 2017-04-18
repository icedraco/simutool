#include <assert.h>
#include <pthread.h>

#include "simucopter-ardupilot.h"


void* simucopter_thread_run(void* p)
{
    struct s_req_msg msg;
    void* sock_rep = bridge_rep_socket(ADDR_ARDUCOPTER);

    assert(sock_rep);

    for (;;) {
        msg = bridge_recv(sock_rep);
        if (msg.flag_ok)
            continue;

        switch (msg.msg_id) {
            case MSG_PING:
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;

            case MSG_GET_ACCEL_X:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_accel_x());
                break;
            case MSG_GET_ACCEL_Y:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_accel_y());
                break;
            case MSG_GET_ACCEL_Z:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_accel_z());
                break;

            case MSG_GET_GYRO_X:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_gyro_x());
                break;
            case MSG_GET_GYRO_Y:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_gyro_y());
                break;
            case MSG_GET_GYRO_Z:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_gyro_z());
                break;

            case MSG_GET_STATE_YAW:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_state_yaw());
                break;
            case MSG_GET_STATE_ROLL:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_state_roll());
                break;
            case MSG_GET_STATE_PITCH:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_state_pitch());
                break;

            case MSG_GET_DESIRED_YAW:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_desired_yaw());
                break;
            case MSG_GET_DESIRED_ROLL:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_desired_roll());
                break;
            case MSG_GET_DESIRED_PITCH:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_desired_pitch());
                break;
            case MSG_GET_DESIRED_THROTTLE:
                bridge_rep_double(msg.socket, msg.msg_id, copter_get_desired_throttle());
                break;

            case MSG_SET_RATE_TARGET_YAW:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;
            case MSG_SET_RATE_TARGET_ROLL:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;
            case MSG_SET_RATE_TARGET_PITCH:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;

            case MSG_SET_MOTORS_YAW:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;
            case MSG_SET_MOTORS_ROLL:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;
            case MSG_SET_MOTORS_PITCH:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;
            case MSG_SET_MOTORS_THROTTLE:
                // TODO
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;

            case MSG_GCS_SEND_TEXT:
                // TODO: implement this (this uses additional data)
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;

            default:
                bridge_rep_confirm(msg.socket, msg.msg_id);
                break;
        }

//        if (msg.data_sz > 0) {
//            // received a bridge_req_set_double()
//            memcpy(&current_value, msg.data, msg.data_sz);
//            bridge_rep_confirm(sock_rep, msg.msg_id);
//        } else {
//            // received a bridge_req_double()
//            current_value = G_TEST_VALUES[i++];
//            bridge_rep_double(sock_rep, msg.msg_id, current_value);
//            if (current_value == 0) i = 0; // reached end of list
//        }
    }

    return NULL;
}

void simucopter_init()
{
    // this function is activated from ArduPilot (system.cpp)
    
    // server-side initialization
    bridge_init();

    pthread_t pt;
    pthread_create(&pt, NULL, simucopter_thread_run, NULL);
}

void simucopter_stop()
{
    // we currently assume that ArduPilot doesn't shut down, and if it
    // does, then it takes all its threads along with it
}

void simucopter_requester_init()
{
    // no requester on ArduPilot - this function isn't run
}

void simucopter_requester_stop()
{
    // no requester on ArduPilot - this function isn't run
}

void simucopter_flight_mode_init()
{
    // this function is activated by ArduPilot each time the "simulink"
    // flight mode is initialized
    TestFlightMode_initialize();
}

void simucopter_flight_mode_run()
{
    // this function is activated by ArduPilot each time its step function
    // is activated
    TestFlightMode_step();
}
