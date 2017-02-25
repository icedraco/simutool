#include "simucopter-requester.h"

#define REQ_DOUBLE(id) __request_double(s_info.zmq_socket, id);
#define REQ_SET_DOUBLE(id, param) __request_set_double(s_info.zmq_socket, id, param);

struct s_adapter_info s_info;


/******************************************************************************\
 * Forward Declarations *******************************************************
\******************************************************************************/

static double __request_double(void* socket, int msg_id);
static void __request_set_double(void* socket, int msg_id, double param);


/******************************************************************************\
 * Primary Functions **********************************************************
\******************************************************************************/

struct s_adapter_info* requester_init() {
    void* context;
    void* socket;

    context = zmq_init(ADAPTER_NUM_ZMQ_IO_THREADS);
    assert(context);

    socket = zmq_socket(context, ZMQ_REQ);
    assert(socket);

    assert(0 == zmq_connect(socket, REP_ADDRESS));

    s_info.zmq_context = context;
    s_info.zmq_socket = socket;
    return &s_info;
}

void requester_stop() {
    zmq_close(s_info.zmq_socket);
    zmq_term(s_info.zmq_context);
}


/******************************************************************************\
 * STUBS **********************************************************************
\******************************************************************************/

double copter_get_accel_x() {
    return REQ_DOUBLE(MSG_GET_ACCEL_X);
}

double copter_get_accel_y() {
    return REQ_DOUBLE(MSG_GET_ACCEL_Y);
}

double copter_get_accel_z() {
    return REQ_DOUBLE(MSG_GET_ACCEL_Z);
}

double copter_get_gyro_x() {
    return REQ_DOUBLE(MSG_GET_GYRO_X);
}

double copter_get_gyro_y() {
    return REQ_DOUBLE(MSG_GET_GYRO_Y);
}

double copter_get_gyro_z() {
    return REQ_DOUBLE(MSG_GET_GYRO_Z);
}

double copter_get_state_yaw() {
    return REQ_DOUBLE(MSG_GET_STATE_YAW);
}

double copter_get_state_roll() {
    return REQ_DOUBLE(MSG_GET_STATE_ROLL);
}

double copter_get_state_pitch() {
    return REQ_DOUBLE(MSG_GET_STATE_PITCH);
}

double copter_get_desired_yaw() {
    return REQ_DOUBLE(MSG_GET_DESIRED_YAW);
}

double copter_get_desired_roll() {
    return REQ_DOUBLE(MSG_GET_DESIRED_ROLL);
}

double copter_get_desired_pitch() {
    return REQ_DOUBLE(MSG_GET_DESIRED_PITCH);
}

double copter_get_desired_throttle() {
    return REQ_DOUBLE(MSG_GET_DESIRED_THROTTLE);
}

void copter_set_rate_target_yaw(double yaw) {
    REQ_SET_DOUBLE(MSG_SET_RATE_TARGET_YAW, yaw);
}

void copter_set_rate_target_pitch(double pitch) {
    REQ_SET_DOUBLE(MSG_SET_RATE_TARGET_PITCH, pitch);
}

void copter_set_rate_target_roll(double roll) {
    REQ_SET_DOUBLE(MSG_SET_RATE_TARGET_ROLL, roll);
}

// see ardupilot/libraries/AP_Motors/AP_Motors_Class.h
void copter_motors_set_roll(double roll) {
    REQ_SET_DOUBLE(MSG_SET_MOTORS_ROLL, roll);
}

void copter_motors_set_pitch(double pitch) {
    REQ_SET_DOUBLE(MSG_SET_MOTORS_PITCH, pitch);
}

void copter_motors_set_yaw(double yaw) {
    REQ_SET_DOUBLE(MSG_SET_MOTORS_YAW, yaw);
}

void copter_motors_set_throttle(double throttle) {
    REQ_SET_DOUBLE(MSG_SET_MOTORS_THROTTLE, throttle);
}

void copter_gcs_send_text(int severity, const char* str) {
    assert(0); // TODO: not yet implemented
}


/*** HELPER FUNCTIONS *********************************************************/

static double __request_double(void* socket, int msg_id) {
    double result;
    int rc;
    int has_data;
    size_t has_data_sz = sizeof(has_data);

    struct s_adapter_meta meta;
    meta.msg_id = msg_id;
    meta.data_sz = 0;

    rc = zmq_send(socket, &meta, sizeof(struct s_adapter_meta), 0);
    if (rc == -1) {
    	perror("__request_double/zmq_send");
        assert(0);
    }

    rc = zmq_recv(socket, &meta, sizeof(struct s_adapter_meta), 0);
    assert(rc != -1);
    assert(meta.msg_id == msg_id);
    assert(meta.data_sz == sizeof(result));

    rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &has_data, &has_data_sz);
    assert(rc != -1);

    rc = zmq_recv(socket, &result, sizeof(result), 0);
    assert(rc != -1);

    return result;
}

static void __request_set_double(void* socket, int msg_id, double param) {
    int rc; 
    int has_data;
    size_t has_data_sz = sizeof(has_data);
    char buffer[16]; // for incoming data (if any) - we'll ignore it for now

    struct s_adapter_meta meta;
    meta.msg_id = msg_id;
    meta.data_sz = sizeof(param);

    rc = zmq_send(socket, &meta, sizeof(struct s_adapter_meta), 0);
    assert(rc != -1);

    // confirmation
    rc = zmq_recv(socket, &meta, sizeof(struct s_adapter_meta), 0);
    assert(rc != -1);
    assert(meta.msg_id == msg_id);
    //assert(meta.data_sz == 0);
    
    // see if we have more data
    rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &has_data, &has_data_sz);
    assert(rc != -1);
    
    if (has_data) {
        rc = zmq_recv(socket, buffer, sizeof(buffer), 0);
        assert(rc != -1);
    }
}
