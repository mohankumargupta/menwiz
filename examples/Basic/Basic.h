#ifndef BASIC_H
#define BASIC_H

#include <Arduino.h>
#include <MENWIZ.h>
#include <LiquidCrystal_I2C.h>

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


int navigation();
void zeroaxis();
void exitMenu();
void myuserscreen();

#endif //BASIC_H

