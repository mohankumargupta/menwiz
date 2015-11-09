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

#define KEYPAD_INTPIN A2
#define KEYPAD_KEYSCOUNT 16
#define KEYPAD_I2CADDRESS 0x70

int navigation();
void zeroaxis();
void exitMenu();
void myuserscreen();
void encoderButtonClicked();
void encoderTurned();

void setLatheMode();
void setMillMode();
void setMetric();
void setImperial();
void doNothing();
void digit1();
void digit2();
void digit3();
void digit4();
void digit5();
void digit6();

void enterButton();
void escapeButton();

void presetX();
void presetY();
void presetZ();

void encoderXinterrupt();
void encoderYinterrupt();
void encoderZinterrupt();

#endif //BASIC_H

