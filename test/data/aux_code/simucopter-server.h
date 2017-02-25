#ifndef __SIMUCOPTER_SERVER_H_
#define __SIMUCOPTER_SERVER_H_

#include <string.h>
#include <assert.h>

#include "simucopter-protocol.h"
#include "simucopter-transport.h"
#include "simucopter-copter.h"

#define LISTENING_ADDRESS "tcp://*:5555"

struct s_adapter_info* server_init();
void server_stop();
void server_step();

#endif
