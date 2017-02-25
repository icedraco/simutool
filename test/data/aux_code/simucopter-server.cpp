#include "simucopter-server.h"

struct s_adapter_info s_info;


/******************************************************************************\
 * Forward Declarations *******************************************************
\******************************************************************************/

static int __return_int(void* socket, struct s_adapter_meta* rep_meta, const int value);
static int __return_double(void* socket, struct s_adapter_meta* rep_meta, const double value);

int server_handle_message(void*, const struct s_adapter_meta* , const void*);


/******************************************************************************\
 * Primary Functions **********************************************************
\******************************************************************************/

struct s_adapter_info* server_init() {
    void* context = zmq_init(ADAPTER_NUM_ZMQ_IO_THREADS);
    void* socket = zmq_socket(context, ZMQ_REP);

    assert(0 == zmq_bind(socket, LISTENING_ADDRESS));

    s_info.zmq_context = context;
    s_info.zmq_socket = socket;
    return &s_info;
}

void server_stop() {
    zmq_unbind(s_info.zmq_socket, LISTENING_ADDRESS);
    zmq_close(s_info.zmq_socket);
    zmq_term(s_info.zmq_context);
}

void server_step() {
    void* socket = s_info.zmq_socket;
    int rc = 0;
    int has_data = 0;
    size_t has_data_sz = sizeof(has_data);
    struct s_adapter_meta req_meta;
    unsigned char data[DATA_BUFFER_SZ];

    req_meta.msg_id = 0;
    req_meta.data_sz = 0;

    // receive a request
    rc = zmq_recv(socket, &req_meta, sizeof(req_meta), 0);
    if (rc == -1)
        return; // bail early

    // receive the data as well, if any
    rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &has_data, &has_data_sz);
    assert(rc != -1);

    if (has_data) {
	rc = zmq_recv(socket, data, sizeof(data), 0);
	assert(rc != -1);
    }

    // process the request
    server_handle_message(socket, &req_meta, data);
}

int server_handle_message(
    void* socket,
    const struct s_adapter_meta* req_meta,
    const void* req_data)
{
    struct s_adapter_meta rep_meta;

    // prepare response
    rep_meta.msg_id = req_meta->msg_id;
    rep_meta.data_sz = 0;

    switch (req_meta->msg_id) {
        case MSG_PING:
            return zmq_send(socket, &rep_meta, sizeof(rep_meta), 0);

        // ---
        case MSG_GET_ACCEL_X:
            return __return_double(socket, &rep_meta, copter_get_accel_x());
        case MSG_GET_ACCEL_Y:
            return __return_double(socket, &rep_meta, copter_get_accel_y());
        case MSG_GET_ACCEL_Z:
            return __return_double(socket, &rep_meta, copter_get_accel_z());

        // ---
        case MSG_GET_GYRO_X:
            return __return_double(socket, &rep_meta, copter_get_gyro_x());
        case MSG_GET_GYRO_Y:
            return __return_double(socket, &rep_meta, copter_get_gyro_y());
        case MSG_GET_GYRO_Z:
            return __return_double(socket, &rep_meta, copter_get_gyro_z());

        // ---
        case MSG_GET_STATE_YAW:
            return __return_double(socket, &rep_meta, copter_get_state_yaw());
        case MSG_GET_STATE_ROLL:
            return __return_double(socket, &rep_meta, copter_get_state_roll());
        case MSG_GET_STATE_PITCH:
            return __return_double(socket, &rep_meta, copter_get_state_pitch());

        // ---
        case MSG_GET_DESIRED_YAW:
            return __return_double(socket, &rep_meta, copter_get_desired_yaw());
        case MSG_GET_DESIRED_ROLL:
            return __return_double(socket, &rep_meta, copter_get_desired_roll());
        case MSG_GET_DESIRED_PITCH:
            return __return_double(socket, &rep_meta, copter_get_desired_pitch());
        case MSG_GET_DESIRED_THROTTLE:
            return __return_double(socket, &rep_meta, copter_get_desired_throttle());

        // ---
        case MSG_SET_RATE_TARGET_YAW:
            return __return_double(socket, &rep_meta, -1); // setters not yet available
        case MSG_SET_RATE_TARGET_ROLL:
            return __return_double(socket, &rep_meta, -1); // setters not yet available
        case MSG_SET_RATE_TARGET_PITCH:
            return __return_double(socket, &rep_meta, -1); // setters not yet available

        // ---
        case MSG_SET_MOTORS_YAW:
            return __return_double(socket, &rep_meta, -1); // setters not yet available
        case MSG_SET_MOTORS_ROLL:
            return __return_double(socket, &rep_meta, -1); // setters not yet available
        case MSG_SET_MOTORS_PITCH:
            return __return_double(socket, &rep_meta, -1); // setters not yet available
        case MSG_SET_MOTORS_THROTTLE:
            return __return_double(socket, &rep_meta, -1); // setters not yet available

        // ---
        case MSG_GCS_SEND_TEXT:
            return __return_int(socket, &rep_meta, -1); // GCS not yet available

        // ---
        default:
            //assert(false); // this shouldn't happen, technically..
            return __return_int(socket, &rep_meta, -1);
    }
}


/*** HELPER FUNCTIONS *********************************************************/

static int __return_int(void* socket, struct s_adapter_meta* rep_meta, const int value) {
    rep_meta->data_sz = sizeof(value);

    int rc = zmq_send(socket, rep_meta, sizeof(struct s_adapter_meta), ZMQ_SNDMORE);
    if (rc != -1)
        rc = zmq_send(socket, &value, rep_meta->data_sz, 0);

    assert(rc != -1);

    return rc;
}

static int __return_double(void* socket, struct s_adapter_meta* rep_meta, const double value) {
    rep_meta->data_sz = sizeof(value);

    int rc = zmq_send(socket, rep_meta, sizeof(struct s_adapter_meta), ZMQ_SNDMORE);
    if (rc != -1)
        rc = zmq_send(socket, &value, rep_meta->data_sz, 0);

    assert(rc != -1);

    return rc;
}
