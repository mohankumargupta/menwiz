Menu library for Arduino LCD with rotary encoder
Highly customised version of MENWIZ library.

PREREQUISITES:

The following libraries need to be installed first:
- EnableInterrupt
- LiquidCrystal_I2C

These are contained in the REQUIREDLIBRARIES.zip file.

NB: The default LiquidCrystal library needs to be moved out of the libraries directory(not simply renamed) or deleted and be replaced by LiquidCrystal_I2C. Once there, rename the library to LiquidCrystal.

INSTALLATION
Install this library like any other. Once copied to the right location, you will find the example sketch.


FILES
Basic.ino : These contain headers for the libraries required. If you need additional libraries, include headers here.
main.cpp: The main setup and loop functions are here.
main.h: You will find constants and funtion declarations here.
Configuration.h: This contains user-configurable settings.

TODO
1. Add an option to Configuration to allow you to choose between LiquidCrystal.h (parallel) and LiquidCrystal_I2C.h (i2C)
2. encoder functionality (both the one attached to LCD and linear encoders XYZ)



