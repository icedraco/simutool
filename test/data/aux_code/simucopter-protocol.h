#ifndef __SIMUCOPTER_PROTOCOL_H_
#define __SIMUCOPTER_PROTOCOL_H_

// Represents a "metadata" frame that's sent over ZMQ before the data itself.
// The metadata contains the data-specific message ID and other info.
struct s_adapter_meta {
    int      msg_id;
    int      data_sz;
};


/*****************************************************************************\
 * Message IDs ***************************************************************
\*****************************************************************************/

#define MSG_PING                  0x0000

// UAV-related messages start at 0x1000

#define MSG_GET_ACCEL_X           0x1001
#define MSG_GET_ACCEL_Y           0x1002
#define MSG_GET_ACCEL_Z           0x1003

#define MSG_GET_GYRO_X            0x1011
#define MSG_GET_GYRO_Y            0x1012
#define MSG_GET_GYRO_Z            0x1013

#define MSG_GET_STATE_YAW         0x1031
#define MSG_GET_STATE_ROLL        0x1032
#define MSG_GET_STATE_PITCH       0x1033

#define MSG_GET_DESIRED_YAW       0x1041
#define MSG_GET_DESIRED_ROLL      0x1042
#define MSG_GET_DESIRED_PITCH     0x1043
#define MSG_GET_DESIRED_THROTTLE  0x1044

#define MSG_SET_RATE_TARGET_YAW   0x1051
#define MSG_SET_RATE_TARGET_ROLL  0x1052
#define MSG_SET_RATE_TARGET_PITCH 0x1053

#define MSG_SET_MOTORS_YAW        0x1061
#define MSG_SET_MOTORS_ROLL       0x1062
#define MSG_SET_MOTORS_PITCH      0x1063
#define MSG_SET_MOTORS_THROTTLE   0x1064

// GCS communication messages start at 0x2000

#define MSG_GCS_SEND_TEXT         0x2001

#endif


