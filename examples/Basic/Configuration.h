#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//Convert Units
#define CONVERT_METRIC 11.8
#define CONVERT_IMPERIAL 301.0
#define CONVERT_SOMETHING 1.0
#define CONVERT_SOMETHING2 1.0
#define CONVERT_SOMETHING3 1.0

// Encoder Ports - XYZ Axis linear encoders 
//#define ENCODERX_PINA 6
//#define ENCODERX_PINB 7
//#define ENCODERX_PINA 2
//#define ENCODERX_PINB 3
//#define ENCODERY_PINA 4
//#define ENCODERY_PINB 5
//#define ENCODERZ_PINA 6
//#define ENCODERZ_PINB 7
#define ENCODER_A 6
#define ENCODER_B 7
#define ENCODER_BUTTON 8

// LCD Encoder 
#define LCD_ENCODER 8

//Comment out the following if you are using I2C LCD
//Uncomment out the following if you are using parallel LCD 
//#define LCD_PARALLEL

#endif //CONFIGURATION_H
