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

#define ENCODERROTATED_NONE 0
#define ENCODERROTATED_CLOCKWISE 1
#define ENCODERROTATED_ANTICLOCKWISE 2

#define KEYPAD_INTPIN A2
#define KEYPAD_KEYSCOUNT 16
#define KEYPAD_I2CADDRESS 0x70

#define CONST_PRESET_X 0
#define CONST_PRESET_Y 1
#define CONST_PRESET_Z 2

#define LATHE_METRIC 0
#define LATHE_IMPERIAL 1
#define MILL_METRIC 2
#define MILL_IMPERIAL 3

#define LATHEMODE_DIAMETER 0
#define LATHEMODE_RADIUS 1 

#define STORE_MULTIPLE 1000
#define STORE_MULTIPLE_FLOAT 1000.0 


int navigation();
void zeroaxis();
void countpulses();
void reset();
void exitMenu();
void myuserscreen();
void encoderButtonClicked();
void encoderTurned();

void setLatheMode();
void setMillMode();
void setMetric();
void setImperial();
void doNothing();
void digit0();
void digit1();
void digit2();
void digit3();
void digit4();
void digit5();
void digit6();
void digit7();
void digit8();
void digit9();
void loadToolButton();
void decimalPointButton();
void storeToolButton();

void load_tool1();
void load_tool2();
void load_tool3();
void load_tool4();

void store_tool1();
void store_tool2();
void store_tool3();
void store_tool4();

void handleDigit(int);

void enterButton();
void escapeButton();

void presetX();
void presetY();
void presetZ();

void encoderXinterrupt();
void encoderYinterrupt();
void encoderZinterrupt();

void showToolsOffsets();
void clearAndHome()
#endif //BASIC_H

