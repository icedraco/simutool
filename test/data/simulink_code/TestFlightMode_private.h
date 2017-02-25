//
// File: TestFlightMode_private.h
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
#ifndef RTW_HEADER_TestFlightMode_private_h_
#define RTW_HEADER_TestFlightMode_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"

// Private macros used by the generated code to access rtModel
#ifndef rtmSetTFinal
# define rtmSetTFinal(rtm, val)        ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               (&(rtm)->Timing.taskTime0)
#endif
#endif                                 // RTW_HEADER_TestFlightMode_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
