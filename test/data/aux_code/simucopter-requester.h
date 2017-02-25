#ifndef __SIMULINK_REQUESTER_H_
#define __SIMULINK_REQUESTER_H_

#include <string.h>
#include <assert.h>

#include "simucopter-protocol.h"
#include "simucopter-transport.h"
#include "simucopter-funcs.h"

#define REP_ADDRESS "tcp://localhost:5555"

struct s_adapter_info* requester_init();
void requester_stop();

#endif
