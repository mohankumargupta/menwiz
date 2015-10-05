#include "Configuration.h"
#include <Wire.h>
#include <EnableInterrupt.h>
#include <LCD.h>
#ifdef LCD_PARALLEL 
  #include <LiquidCrystal.h>
#else
  #include <LiquidCrystal_I2C.h>
#endif
#include <MENWIZ.h>