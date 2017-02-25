//
// File: TestFlightMode.h
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
#ifndef RTW_HEADER_TestFlightMode_h_
#define RTW_HEADER_TestFlightMode_h_
#include <float.h>
#include <string.h>
#include <stddef.h>
#ifndef TestFlightMode_COMMON_INCLUDES_
# define TestFlightMode_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_extmode.h"
#include "sysran_types.h"
#include "dt_info.h"
#include "ext_work.h"
#include "simucopter.h"
#endif                                 // TestFlightMode_COMMON_INCLUDES_

#include "TestFlightMode_types.h"

// Shared type includes
#include "multiword_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWExtModeInfo
# define rtmGetRTWExtModeInfo(rtm)     ((rtm)->extModeInfo)
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

// Block signals (auto storage)
typedef struct {
    real_T Acceleration_o1;            // '<Root>/Acceleration'
    real_T Acceleration_o2;            // '<Root>/Acceleration'
    real_T Acceleration_o3;            // '<Root>/Acceleration'
    real_T CopterState_o1;             // '<Root>/Copter State'
    real_T CopterState_o2;             // '<Root>/Copter State'
    real_T CopterState_o3;             // '<Root>/Copter State'
} B_TestFlightMode_T;

// Block states (auto storage) for system '<Root>'
typedef struct {
    struct {
        void *LoggedData[3];
    } ACCEL_PWORK;                     // '<Root>/ACCEL'

    struct {
        void *LoggedData[3];
    } YRP_PWORK;                       // '<Root>/YRP'

    void *Acceleration_PWORK;          // '<Root>/Acceleration'
    void *Motors_PWORK;                // '<Root>/Motors'
    void *CopterState_PWORK;           // '<Root>/Copter State'
    Accel_TestFlightMode_T obj;        // '<Root>/Acceleration'
    Copter_Self_State_TestFlightM_T obj_l0mr0qnxdf;// '<Root>/Copter State'
    Motors_TestFlightMode_T obj_mozkfpvysz;// '<Root>/Motors'
} DW_TestFlightMode_T;

// Parameters (auto storage)
struct P_TestFlightMode_T_ {
    real_T Constant2_Value;            // Expression: 1
                                       //  Referenced by: '<Root>/Constant2'

    real_T Constant1_Value;            // Expression: 0
                                       //  Referenced by: '<Root>/Constant1'

    real_T Constant_Value;             // Expression: 0
                                       //  Referenced by: '<Root>/Constant'

};

// Real-time Model Data Structure
struct tag_RTM_TestFlightMode_T {
    const char_T *errorStatus;
    RTWExtModeInfo *extModeInfo;

    //
    //  Sizes:
    //  The following substructure contains sizes information
    //  for many of the model attributes such as inputs, outputs,
    //  dwork, sample times, etc.

    struct {
        uint32_T checksums[4];
    } Sizes;

    //
    //  SpecialInfo:
    //  The following substructure contains special information
    //  related to other components that are dependent on RTW.

    struct {
        const void *mappingInfo;
    } SpecialInfo;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
        time_T taskTime0;
        uint32_T clockTick0;
        time_T stepSize0;
        time_T tFinal;
        boolean_T stopRequestedFlag;
    } Timing;
};

// Block parameters (auto storage)
#ifdef __cplusplus

extern "C" {

#endif

    extern P_TestFlightMode_T TestFlightMode_P;

#ifdef __cplusplus

}
#endif

// Block signals (auto storage)
extern B_TestFlightMode_T TestFlightMode_B;

// Block states (auto storage)
extern DW_TestFlightMode_T TestFlightMode_DW;

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

    // Model entry point functions
    extern void TestFlightMode_initialize(void);
    extern void TestFlightMode_step(void);
    extern void TestFlightMode_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

    extern RT_MODEL_TestFlightMode_T *const TestFlightMode_M;

#ifdef __cplusplus

}
#endif

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'TestFlightMode'

#endif                                 // RTW_HEADER_TestFlightMode_h_

//
// File trailer for generated code.
//
// [EOF]
//
