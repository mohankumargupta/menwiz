#ifndef BASIC_H
#define BASIC_H

#include "Configuration.h"
#include <Arduino.h>
#include <MENWIZ.h>
#ifdef LCD_PARALLEL
  #include <LiquidCrystal.h>
#else
  #include <LiquidCrystal_I2C.h>
#endif
#ifdef TRELLIS_KEYPAD
  #include "Adafruit_Trellis.h"
#endif
#define LIBCALL_ENABLEINTERRUPT
#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


// Lathe/mill
#define LATHE 0
#define MILL 1

//Units
#define METRIC 0
#define IMPERIAL 1
#define SOMETHING 2
#define SOMETHING2 3
#define SOMETHING3 4

#define TIMEOUT_DELAY 10000

#define PRESET_MIN 0.0
#define PRESET_MAX 1000.0
#define PRESET_INCREMENT 0.5

#define ENCODERROTATED_NONE 0
#define ENCODERROTATED_CLOCKWISE 1
#define ENCODERROTATED_ANTICLOCKWISE 2

int navigation();
void zeroaxis();
void exitMenu();
void myuserscreen();
void encoderButtonClicked();
void encoderTurned();

#endif //BASIC_H

