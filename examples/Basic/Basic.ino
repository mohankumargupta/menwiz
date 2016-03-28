//Version 28.03.2016.356
#include "Configuration.h"
#include <Wire.h>
#include <EnableInterrupt.h>
#include <LCD.h>
#ifdef LCD_PARALLEL 
  #include <LiquidCrystal.h>
#else
  #include <LiquidCrystal_I2C.h>
#endif
#ifdef TRELLIS_KEYPAD
  #include "Adafruit_Trellis.h"
#endif
#include <MENWIZ.h>
