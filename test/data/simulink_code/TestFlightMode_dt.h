//
//  TestFlightMode_dt.h
//
//  Code generation for model "TestFlightMode".
//
//  Model version              : 1.35
//  Simulink Coder version : 8.10 (R2016a) 10-Feb-2016
//  C++ source code generated on : Thu Feb 23 20:54:37 2017
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objectives: Unspecified
//  Validation result: Not run


#include "ext_types.h"

// data type size table
static uint_T rtDataTypeSizes[] = {
    sizeof(real_T),
    sizeof(real32_T),
    sizeof(int8_T),
    sizeof(uint8_T),
    sizeof(int16_T),
    sizeof(uint16_T),
    sizeof(int32_T),
    sizeof(uint32_T),
    sizeof(boolean_T),
    sizeof(fcn_call_T),
    sizeof(int_T),
    sizeof(pointer_T),
    sizeof(action_T),
    2*sizeof(uint32_T),
    sizeof(Accel_TestFlightMode_T),
    sizeof(Copter_Self_State_TestFlightM_T),
    sizeof(Motors_TestFlightMode_T)
};

// data type name table
static const char_T * rtDataTypeNames[] = {
    "real_T",
    "real32_T",
    "int8_T",
    "uint8_T",
    "int16_T",
    "uint16_T",
    "int32_T",
    "uint32_T",
    "boolean_T",
    "fcn_call_T",
    "int_T",
    "pointer_T",
    "action_T",
    "timer_uint32_pair_T",
    "Accel_TestFlightMode_T",
    "Copter_Self_State_TestFlightM_T",
    "Motors_TestFlightMode_T"
};

// data type transitions for block I/O structure
static DataTypeTransition rtBTransitions[] = {
    { (char_T *)(&TestFlightMode_B.Acceleration_o1), 0, 0, 6 }
    ,

    { (char_T *)(&TestFlightMode_DW.ACCEL_PWORK.LoggedData[0]), 11, 0, 9 },

    { (char_T *)(&TestFlightMode_DW.obj), 14, 0, 1 },

    { (char_T *)(&TestFlightMode_DW.obj_l0mr0qnxdf), 15, 0, 1 },

    { (char_T *)(&TestFlightMode_DW.obj_mozkfpvysz), 16, 0, 1 }
};

// data type transition table for block I/O structure
static DataTypeTransitionTable rtBTransTable = {
    5U,
    rtBTransitions
};

// data type transitions for Parameters structure
static DataTypeTransition rtPTransitions[] = {
    { (char_T *)(&TestFlightMode_P.Constant2_Value), 0, 0, 3 }
};

// data type transition table for Parameters structure
static DataTypeTransitionTable rtPTransTable = {
    1U,
    rtPTransitions
};

// [EOF] TestFlightMode_dt.h
