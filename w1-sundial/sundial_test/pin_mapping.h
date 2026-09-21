#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

// --------------------------------------------------
// Shared CD4051 select pins
// --------------------------------------------------

const int S0_PIN = 2;
const int S1_PIN = 3;
const int S2_PIN = 4;

// CD4051 common outputs
const int MUX1_OUTPUT = A0;
const int MUX2_OUTPUT = A1;

const int NUM_CHANNELS = 8;
const int NUM_SAMPLES = 5;
const int ADC_MAX = 4095;

// --------------------------------------------------
// MUX1 hour mapping
//
// Index:
// 0 = X0, physical pin 13
// 1 = X1, physical pin 14
// 2 = X2, physical pin 15
// 3 = X3, physical pin 12
// 4 = X4, physical pin 1
// 5 = X5, physical pin 5
// 6 = X6, physical pin 2
// 7 = X7, physical pin 4
// --------------------------------------------------

const int HOUR_FOR_MUX1[NUM_CHANNELS] = {
  10,  // X0, physical pin 13
  11,  // X1, physical pin 14
  12, // X2, physical pin 15
  9,  // X3, physical pin 12
  1,  // X4, e4physical pin 1
  4,  // X5, physical pin 5
  2, // X6, physical pin 2ooo
  3  // X7, physical pin 4
};

// --------------------------------------------------
// MUX2 hour mapping
//
// From your handwritten chart:
// Hour 4  → physical pin 15 → X2
// Hour 5  → physical pin 14 → X1
// Hour 6  → physical pin 2  → X6
// Hour 7  → physical pin 4  → X7
//
// Zero means unused.
// --------------------------------------------------

const int HOUR_FOR_MUX2[NUM_CHANNELS] = {
  0, // X0, physical pin 13: unused
  7, // X1, physical pin 14
  8, // X2, physical pin 15
  0, // X3, physical pin 12: unused
  5, // X4, physical pin 1: 
  0, // X5, physical pin 5: unused
  6, // X6, physical pin 2
  0,  // X7, physical pin 4: unused
};

#endif