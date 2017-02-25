//
// File: ert_main.cpp
//
// Code generated for Simulink model 'TestFlightMode'.
//
// Model version                  : 1.35
// Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
// C/C++ source code generated on : Thu Feb 23 20:54:37 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <stdio.h>
#include <stdlib.h>
#include "TestFlightMode.h"
#include "TestFlightMode_private.h"
#include "rtwtypes.h"
#include "limits.h"
#include "linuxinitialize.h"

// Function prototype declaration
void exitTask(int sig);
void terminateTask(void *arg);
void baseRateTask(void *arg);
void subrateTask(void *arg);
volatile boolean_T runModel = true;
sem_t stopSem;
sem_t baserateTaskSem;
pthread_t schedulerThread;
pthread_t baseRateThread;
unsigned long threadJoinStatus[8];
int terminatingmodel = 0;
void baseRateTask(void *arg)
{
    runModel = (rtmGetErrorStatus(TestFlightMode_M) == (NULL)) &&
        !rtmGetStopRequested(TestFlightMode_M);
    while (runModel) {
        sem_wait(&baserateTaskSem);

        // External mode
        {
            boolean_T rtmStopReq = false;
            rtExtModePauseIfNeeded(TestFlightMode_M->extModeInfo, 1, &rtmStopReq);
            if (rtmStopReq) {
                rtmSetStopRequested(TestFlightMode_M, true);
            }

            if (rtmGetStopRequested(TestFlightMode_M) == true) {
                rtmSetErrorStatus(TestFlightMode_M, "Simulation finished");
                break;
            }
        }

        // External mode
        {
            boolean_T rtmStopReq = false;
            rtExtModeOneStep(TestFlightMode_M->extModeInfo, 1, &rtmStopReq);
            if (rtmStopReq) {
                rtmSetStopRequested(TestFlightMode_M, true);
            }
        }

        TestFlightMode_step();

        // Get model outputs here
        rtExtModeCheckEndTrigger();
        runModel = (rtmGetErrorStatus(TestFlightMode_M) == (NULL)) &&
            !rtmGetStopRequested(TestFlightMode_M);
    }

    runModel = 0;
    terminateTask(arg);
    pthread_exit((void *)0);
}

void exitTask(int sig)
{
    rtmSetErrorStatus(TestFlightMode_M, "stopping the model");
}

void terminateTask(void *arg)
{
    terminatingmodel = 1;
    printf("**terminating the model**\n");
    fflush(stdout);

    {
        int ret;
        runModel = 0;
    }

    rtExtModeShutdown(1);

    // Disable rt_OneStep() here

    // Terminate model
    TestFlightMode_terminate();
    sem_post(&stopSem);
}

int main(int argc, char **argv)
{
    printf("**starting the model**\n");
    fflush(stdout);
    rtmSetErrorStatus(TestFlightMode_M, 0);
    rtExtModeParseArgs(argc, (const char_T **)argv, NULL);

    // Initialize model
    TestFlightMode_initialize();

    // External mode
    rtSetTFinalForExtMode(&rtmGetTFinal(TestFlightMode_M));
    rtExtModeCheckInit(1);

    {
        boolean_T rtmStopReq = false;
        rtExtModeWaitForStartPkt(TestFlightMode_M->extModeInfo, 1, &rtmStopReq);
        if (rtmStopReq) {
            rtmSetStopRequested(TestFlightMode_M, true);
        }
    }

    rtERTExtModeStartMsg();

    // Call RTOS Initialization funcation
    myRTOSInit(0.2, 0);

    // Wait for stop semaphore
    sem_wait(&stopSem);
    return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
