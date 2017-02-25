//
// File: TestFlightMode.cpp
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
#include "TestFlightMode.h"
#include "TestFlightMode_private.h"
#include "TestFlightMode_dt.h"

// Block signals (auto storage)
B_TestFlightMode_T TestFlightMode_B;

// Block states (auto storage)
DW_TestFlightMode_T TestFlightMode_DW;

// Real-time model
RT_MODEL_TestFlightMode_T TestFlightMode_M_;
RT_MODEL_TestFlightMode_T *const TestFlightMode_M = &TestFlightMode_M_;

// Model step function
void TestFlightMode_step(void)
{
    // MATLABSystem: '<Root>/Acceleration' incorporates:
    //   Start for MATLABSystem: '<Root>/Acceleration'

    TestFlightMode_B.Acceleration_o1 = copter_get_accel_x();
    TestFlightMode_B.Acceleration_o2 = copter_get_accel_y();
    TestFlightMode_B.Acceleration_o3 = copter_get_accel_z();

    // Start for MATLABSystem: '<Root>/Motors' incorporates:
    //   Constant: '<Root>/Constant'
    //   Constant: '<Root>/Constant1'
    //   Constant: '<Root>/Constant2'
    //   MATLABSystem: '<Root>/Motors'

    if ((TestFlightMode_P.Constant2_Value >= -1.0) &&
            (TestFlightMode_P.Constant2_Value <= 1.0)) {
        copter_motors_set_roll(TestFlightMode_P.Constant2_Value);
    }

    if ((TestFlightMode_P.Constant1_Value >= -1.0) &&
            (TestFlightMode_P.Constant1_Value <= 1.0)) {
        copter_motors_set_pitch(TestFlightMode_P.Constant1_Value);
    }

    if ((TestFlightMode_P.Constant1_Value >= -1.0) &&
            (TestFlightMode_P.Constant1_Value <= 1.0)) {
        copter_motors_set_yaw(TestFlightMode_P.Constant1_Value);
    }

    if ((TestFlightMode_P.Constant_Value >= 0.0) &&
            (TestFlightMode_P.Constant_Value <= 1.0)) {
        copter_motors_set_throttle(TestFlightMode_P.Constant_Value);
    }

    // End of Start for MATLABSystem: '<Root>/Motors'

    // MATLABSystem: '<Root>/Copter State' incorporates:
    //   Start for MATLABSystem: '<Root>/Copter State'

    TestFlightMode_B.CopterState_o1 = copter_get_state_yaw();
    TestFlightMode_B.CopterState_o2 = copter_get_state_roll();
    TestFlightMode_B.CopterState_o3 = copter_get_state_pitch();

    // External mode
    rtExtModeUploadCheckTrigger(1);

    {                                  // Sample time: [0.2s, 0.0s]
        rtExtModeUpload(0, TestFlightMode_M->Timing.taskTime0);
    }

    // signal main to stop simulation
    {                                  // Sample time: [0.2s, 0.0s]
        if ((rtmGetTFinal(TestFlightMode_M)!=-1) &&
            !((rtmGetTFinal(TestFlightMode_M)-TestFlightMode_M->Timing.taskTime0)
              > TestFlightMode_M->Timing.taskTime0 * (DBL_EPSILON))) {
            rtmSetErrorStatus(TestFlightMode_M, "Simulation finished");
        }

        if (rtmGetStopRequested(TestFlightMode_M)) {
            rtmSetErrorStatus(TestFlightMode_M, "Simulation finished");
        }
    }

    // Update absolute time for base rate
    // The "clockTick0" counts the number of times the code of this task has
    //  been executed. The absolute time is the multiplication of "clockTick0"
    //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
    //  overflow during the application lifespan selected.

    TestFlightMode_M->Timing.taskTime0 =
        (++TestFlightMode_M->Timing.clockTick0) *
        TestFlightMode_M->Timing.stepSize0;
}

// Model initialize function
void TestFlightMode_initialize(void)
{
    // Registration code

    // initialize real-time model
    (void) memset((void *)TestFlightMode_M, 0,
                  sizeof(RT_MODEL_TestFlightMode_T));
    rtmSetTFinal(TestFlightMode_M, -1);
    TestFlightMode_M->Timing.stepSize0 = 0.2;

    // External mode info
    TestFlightMode_M->Sizes.checksums[0] = (487021518U);
    TestFlightMode_M->Sizes.checksums[1] = (3058992413U);
    TestFlightMode_M->Sizes.checksums[2] = (2961386737U);
    TestFlightMode_M->Sizes.checksums[3] = (2777301667U);

    {
        static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
        static RTWExtModeInfo rt_ExtModeInfo;
        static const sysRanDType *systemRan[4];
        TestFlightMode_M->extModeInfo = (&rt_ExtModeInfo);
        rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
        systemRan[0] = &rtAlwaysEnabled;
        systemRan[1] = &rtAlwaysEnabled;
        systemRan[2] = &rtAlwaysEnabled;
        systemRan[3] = &rtAlwaysEnabled;
        rteiSetModelMappingInfoPtr(TestFlightMode_M->extModeInfo,
            &TestFlightMode_M->SpecialInfo.mappingInfo);
        rteiSetChecksumsPtr(TestFlightMode_M->extModeInfo,
                            TestFlightMode_M->Sizes.checksums);
        rteiSetTPtr(TestFlightMode_M->extModeInfo, rtmGetTPtr(TestFlightMode_M));
    }

    // block I/O
    (void) memset(((void *) &TestFlightMode_B), 0,
                  sizeof(B_TestFlightMode_T));

    // states (dwork)
    (void) memset((void *)&TestFlightMode_DW, 0,
                  sizeof(DW_TestFlightMode_T));

    // data type transition information
    {
        static DataTypeTransInfo dtInfo;
        (void) memset((char_T *) &dtInfo, 0,
                      sizeof(dtInfo));
        TestFlightMode_M->SpecialInfo.mappingInfo = (&dtInfo);
        dtInfo.numDataTypes = 17;
        dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
        dtInfo.dataTypeNames = &rtDataTypeNames[0];

        // Block I/O transition table
        dtInfo.BTransTable = &rtBTransTable;

        // Parameters transition table
        dtInfo.PTransTable = &rtPTransTable;
    }

    // Start for MATLABSystem: '<Root>/Acceleration'
    TestFlightMode_DW.obj.isInitialized = 0;
    TestFlightMode_DW.obj.isInitialized = 1;

    // Start for MATLABSystem: '<Root>/Motors'
    TestFlightMode_DW.obj_mozkfpvysz.isInitialized = 0;
    TestFlightMode_DW.obj_mozkfpvysz.isInitialized = 1;

    // Start for MATLABSystem: '<Root>/Copter State'
    TestFlightMode_DW.obj_l0mr0qnxdf.isInitialized = 0;
    TestFlightMode_DW.obj_l0mr0qnxdf.isInitialized = 1;

    // user code (Initialize function Body)
    simucopter_requester_init();
}

// Model terminate function
void TestFlightMode_terminate(void)
{
    // user code (Terminate function Body)
    simucopter_requester_stop();

    // Start for MATLABSystem: '<Root>/Acceleration' incorporates:
    //   Terminate for MATLABSystem: '<Root>/Acceleration'

    if (TestFlightMode_DW.obj.isInitialized == 1) {
        TestFlightMode_DW.obj.isInitialized = 2;
    }

    // End of Start for MATLABSystem: '<Root>/Acceleration'

    // Start for MATLABSystem: '<Root>/Motors' incorporates:
    //   Terminate for MATLABSystem: '<Root>/Motors'

    if (TestFlightMode_DW.obj_mozkfpvysz.isInitialized == 1) {
        TestFlightMode_DW.obj_mozkfpvysz.isInitialized = 2;
    }

    // End of Start for MATLABSystem: '<Root>/Motors'

    // Start for MATLABSystem: '<Root>/Copter State' incorporates:
    //   Terminate for MATLABSystem: '<Root>/Copter State'

    if (TestFlightMode_DW.obj_l0mr0qnxdf.isInitialized == 1) {
        TestFlightMode_DW.obj_l0mr0qnxdf.isInitialized = 2;
    }

    // End of Start for MATLABSystem: '<Root>/Copter State'
}

//
// File trailer for generated code.
//
// [EOF]
//
