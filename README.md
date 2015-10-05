#Description

Menu library for Arduino LCD with rotary encoder
Highly customised version of MENWIZ library (https://github.com/brunialti/MENWIZ_1_3_2)

#PREREQUISITES:

The following libraries need to be installed first:
- EnableInterrupt
- LiquidCrystal_I2C

These are contained in the REQUIREDLIBRARIES.zip file.

Libraries can be installed through Arduino IDE, but my recommmended way is to do it the manual way. The libraries folder is C:\Program Files\Arduino\libraries.
Unzip the zip file into this folder.

NB: The default LiquidCrystal library needs to be moved out of the libraries directory(not simply renamed, eg. move it up one directory level) or deleted and be replaced by LiquidCrystal_I2C. Once there, rename the library to LiquidCrystal.

#INSTALLATION

Install this library like any other. You can use the Arduino IDE for this step if you prefer.There is an example sketch included.

#FILES IN EXAMPLE SKETCH
Basic.ino : These contain headers for the libraries required. If you need additional libraries, include headers here.
Basic_main.cpp: The main setup and loop functions are here.
main.h: You will find constants and function declarations here.
Configuration.h: This contains user-configurable settings.
README.md this file

#TODO
1. Add an option to Configuration to allow you to choose between LiquidCrystal.h (parallel) and LiquidCrystal_I2C.h (i2C)
2. encoder functionality (both the one attached to LCD and linear encoders XYZ)



