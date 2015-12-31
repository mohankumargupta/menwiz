#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//Convert Units
#define CONVERT_METRIC 11.8
#define CONVERT_IMPERIAL 301.0
#define CONVERT_SOMETHING 1.0
#define CONVERT_SOMETHING2 1.0
#define CONVERT_SOMETHING3 1.0

// Encoder Ports - XYZ Axis linear encoders 
#define ENCODERX_PINA 2
#define ENCODERX_PINB 3
#define ENCODERY_PINA 4
#define ENCODERY_PINB 5
#define ENCODERZ_PINA 6
#define ENCODERZ_PINB 7

#define ENCODER_A 8
#define ENCODER_B 9
#define ENCODER_BUTTON 10

//Comment out the following if you are using I2C LCD
//Uncomment out the following if you are using parallel LCD 
//#define LCD_PARALLEL

// Only relevant if you are using parallel I2C, no effect on I2C
#define LCD_ADDRESS 0x27
#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

// Uncomment if your LCD is 16x2
//#define LCD_SIXTEENBYTWO

// Uncomment for keypad support
#define TRELLIS_KEYPAD 

#define PRESET_MIN 0.0
#define PRESET_MAX 1000.0
#define PRESET_INCREMENT 0.1

#endif //CONFIGURATION_H
