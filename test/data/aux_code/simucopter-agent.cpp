//
// simucopter.h implementation: agent-side (Simulink)
//

#include "simucopter.h"
#include "simucopter-requester.h"

void simucopter_init()
{
}

void simucopter_stop()
{
}

void simucopter_requester_init()
{
    // executed by flight mode init function
    requester_init();
}

void simucopter_requester_stop()
{
    // executed by flight mode terminate function
    requester_stop();
}

void simucopter_flight_mode_init()
{
    // this is executed by ArduPilot - agent doesn't touch this
}

void simucopter_flight_mode_run()
{
    // this is executed by ArduPilot - agent doesn't touch this
}
