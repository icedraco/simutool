//
// simucopter.h implementation: server-side (ArduPilot)
//

#include "simucopter.h"
#include "simucopter-server.h"

void* simucopter_thread_run(void* p)
{
    // TODO: Prioritize this thread to low
    server_init();
    for (;;) {
        server_step();
    }
}

void simucopter_init()
{
    // server-side initialization
    pthread_t pt;
    pthread_create(&pt, NULL, simucopter_thread_run, NULL);
}

void simucopter_stop()
{
    // TODO
}

void simucopter_requester_init()
{
    // no requester on ArduPilot
}

void simucopter_requester_stop()
{
    // no requester on ArduPilot
}

void simucopter_flight_mode_init()
{
    TestFlightMode_initialize();
}

void simucopter_flight_mode_run()
{
    TestFlightMode_step();
}
