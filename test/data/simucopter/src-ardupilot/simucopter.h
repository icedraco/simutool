#ifndef ARDUPILOT_SIMUCOPTER_H
#define ARDUPILOT_SIMUCOPTER_H

#include <pthread.h>
#include "simucopter-copter.h"
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
