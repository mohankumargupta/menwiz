#ifndef BASIC_H
#define BASIC_H

#include <Arduino.h>
#include <MENWIZ.h>
#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>
#define LIBCALL_ENABLEINTERRUPT
#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include "Configuration.h"

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


int navigation();
void zeroaxis();
void exitMenu();
void myuserscreen();
void encoderButtonClicked();

#endif //BASIC_H

