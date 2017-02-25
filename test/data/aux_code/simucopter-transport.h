#ifndef __SIMUCOPTER_TRANSPORT_H_
#define __SIMUCOPTER_TRANSPORT_H_

/*** Data Transport - Simulink-ArduPilot Bridge
 *
 * Responsible for delivering data among the Simulink-ArduPilot Bridge
 * components.
 *
 * This particular implementation uses the ZeroMQ library and a pair of sockets,
 * one of which is in REQ (request) mode, while the other is in REP (reply)
 * mode.
 *
 * Last Update: 13-Jan-2017
 * Author:
 *   Yaroslav Sheremet <icedragon@quickfox.org>
 */

#include <zmq.h>

#define DATA_BUFFER_SZ 1024

#define ADAPTER_NUM_ZMQ_IO_THREADS 1


struct s_adapter_info {
    void* zmq_context;
    void* zmq_socket;
};

#endif
