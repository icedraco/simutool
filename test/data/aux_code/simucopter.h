#ifndef ARDUPILOT_SIMUCOPTER_H
#define ARDUPILOT_SIMUCOPTER_H

#ifndef SIMULINK_AGENT
// ArduPilot -> prepare bridge server libs
#include <pthread.h>
#include "simucopter-server.h"
#else
// Simulink Agent -> prepare bridge client/requester
#include "simucopter-requester.h"
#endif

#include "simucopter-funcs.h"
#include "Copter.h"

// TODO: DETERMINE THE NAME OF THE H FILE!
#include "TestFlightMode.h"

struct copter_envelope {
    Copter copter;
    copter_envelope(Copter& c) : copter(c) {}
};

extern void simucopter_init();
extern void simucopter_stop();
void simucopter_requester_init();
void simucopter_requester_stop();
void simucopter_flight_mode_init();
void simucopter_flight_mode_run();

#endif //ARDUPILOT_SIMUCOPTER_H
