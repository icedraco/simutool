//
// simucopter.h implementation: agent-side (Simulink)
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
    // executed by flight mode init function: SomeFlightMode_init()

    uid_t uid;
    
    // --> find the name of current executable (i.e., flight mode name hint)
    char exe[1024];
    ssize_t len = readlink("/proc/self/exe", exe, 1024);
    if (len != -1)
    {
        exe[len] = '\0';
    }
    else
    {
        exe[0] = '\0';
        perror("readlink");
        exit(1);
    }
        
    // --> launch simutool
    pid_t pid = fork();
    
    // fork() failed
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    
    // this is a child process
    if (pid == 0) {
        // NOTE: USE ABSOLUTE PATHS!
        execl("/usr/bin/python2.7", "python", "/home/pi/simucopter/simutool/simutool.py", "deploy", exe, (char*)0);

        // execl() should not return, but if it does...
        perror("execl");
        exit(2);
    }
    
    requester_init();
}

void simucopter_requester_stop()
{
    // executed by flight mode terminate function: SomeFlightMode_terminate()
    requester_stop();
    
    // TODO: Kill child subprocess of ArduPilot and wait for it to exit
}

void simucopter_flight_mode_init()
{
    // this is executed by ArduPilot - agent doesn't touch this
}

void simucopter_flight_mode_run()
{
    // this is executed by ArduPilot - agent doesn't touch this
}
