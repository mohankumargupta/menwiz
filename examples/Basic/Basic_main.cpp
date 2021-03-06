#include "main.h"

#ifdef LCD_PARALLEL
  LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
#else
  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif

#ifdef TRELLIS_KEYPAD
  Adafruit_Trellis _keypad = Adafruit_Trellis();
  Adafruit_TrellisSet keypad =  Adafruit_TrellisSet(&_keypad);
  void (*keyButtonAction[KEYPAD_KEYSCOUNT])() = {presetX, digit1, digit2, digit3, presetY, digit4, digit5, digit6,presetZ,digit7, digit8, digit9,loadToolButton,digit0,decimalPointButton,storeToolButton};
#endif

bool perform_zeroing_function = true;
volatile int count=0;
volatile int state;
long oldPosition = -999;
long newPosition = 0;

const char *METRICIMP_LABEL[] = {"METRIC ", "IMPERIAL "};
const char *DIARAD_LABEL[] = {"DIA ","RAD "};

bool displayUsrScreenImmediately = false;
char lcdchars[80];
char buf[20];

//bool pressEnterKey = false, pressEscapeKey = false;

bool encoderClicked = false;
volatile int encoderRotated = ENCODERROTATED_NONE;

menwiz tree;

float longCountX=0, longCountY=0, longCountZ=0;
volatile long longCountXEncoder=0L, longCountYEncoder=0L, longCountZEncoder=0L; 
float *ptrPresetCount = &longCountX;
//volatile float x,y,z;
int current_preset = CONST_PRESET_X;
int current_preset_pos = 0;
int presets[] = {CONVERTX_METRIC, CONVERTY_METRIC, CONVERTZ_METRIC};
float preset_x=-0.5, preset_y=-0.5, preset_z=-0.5; 
_menu *r,*s0,*s1,*s2, *s3, *s4, *s5, *q4, *p1,*p2,*p3,*p4;
int machine_mode = LATHE_METRIC;
int lathe_mode = LATHEMODE_DIAMETER;
bool loadtool_mode = false;
bool storetool_mode = false;
long tool_x[10];
long tool_y[10];
int current_tool_selection = 1;
int touchoff = TOUCHX;

float convertx = CONVERTX_METRIC;
float converty = CONVERTY_METRIC;
float convertz = CONVERTZ_METRIC;

float CONVERTX_IMPERIAL = CONVERTX_METRIC / 25.4;
float CONVERTY_IMPERIAL = CONVERTY_METRIC / 25.4;
float CONVERTZ_IMPERIAL = CONVERTZ_METRIC / 25.4;

float convertx_preset, converty_preset, convertz_preset;
bool countingpulsesmode = false;

void setup() {
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
  enableInterrupt(ENCODER_BUTTON, encoderButtonClicked ,RISING);
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  enableInterrupt(ENCODER_A, encoderTurned, CHANGE);
  enableInterrupt(ENCODER_B, encoderTurned, CHANGE);

  pinMode(ENCODERX_PINA, INPUT_PULLUP);
  pinMode(ENCODERX_PINB, INPUT_PULLUP);
  pinMode(ENCODERY_PINA, INPUT_PULLUP);
  pinMode(ENCODERY_PINB, INPUT_PULLUP);
  pinMode(ENCODERZ_PINA, INPUT_PULLUP);
  pinMode(ENCODERZ_PINB, INPUT_PULLUP);

  enableInterrupt(ENCODERX_PINA, encoderXinterrupt, CHANGE);
  enableInterrupt(ENCODERY_PINA, encoderYinterrupt, CHANGE);
  enableInterrupt(ENCODERZ_PINA, encoderZinterrupt, CHANGE);

  #ifdef TRELLIS_KEYPAD
    pinMode(KEYPAD_INTPIN, INPUT_PULLUP);
    keypad.begin(KEYPAD_I2CADDRESS);
  #endif

 // Serial.begin(115200);

 // clearAndHome();


  #ifdef LCD_SIXTEENBYTWO   
    tree.begin(&lcd,16,2); 
  #else
    tree.begin(&lcd,20,4);
  #endif
  lcd.clear();
  strcpy(lcdchars, "");

  r = tree.addMenu(MW_ROOT,NULL,F("MAIN MENU")); 
      s0 = tree.addMenu(MW_VAR, r, F("Info Screen"));
           s0->addVar(MW_ACTION, exitMenu);
           s0->setBehaviour(MW_ACTION_CONFIRM, false);  
      s1 = tree.addMenu(MW_VAR, r, F("Change Mode"));
           s1->addVar(MW_LIST, &machine_mode);
           s1->setBehaviour(MW_ACTION_CONFIRM, false); 
           s1->addItem(MW_LIST, F("LATHE(METRIC)"));
           s1->addItem(MW_LIST, F("LATHE(IMPERIAL)"));
           s1->addItem(MW_LIST, F("MILL(METRIC)"));
           s1->addItem(MW_LIST, F("MILL(IMPERIAL)"));          
      s2 = tree.addMenu(MW_VAR, r, F("Lathe Mode"));
           s2->addVar(MW_LIST, &lathe_mode);
           s2->setBehaviour(MW_ACTION_CONFIRM, false); 
           s2->addItem(MW_LIST, F("DIAMETER"));
           s2->addItem(MW_LIST, F("RADIUS"));  
      s4 = tree.addMenu(MW_VAR, r, F("Factory Reset"));    
           s4->addVar(MW_ACTION, reset);
           s4->setBehaviour(MW_ACTION_CONFIRM, false);
      s5 = tree.addMenu(MW_VAR, r, F("Count Pulses"));
           s5->addVar(MW_ACTION, countpulses);
           s5->setBehaviour(MW_ACTION_CONFIRM, false);


      
  tree.addUsrScreen(myuserscreen,TIMEOUT_DELAY);
  tree.addUsrNav(navigation,6);
  tree.showUsrScreen();
  
  lcd.home();  //  splash screen
  lcd.print(" DRO Menwiz ");
  lcd.setCursor(2,2);           // go to the next line
  lcd.print(MENWIZ_VERSION_NUMBER);  //Update version No. here
  delay(2000);

  #ifdef TRELLIS_KEYPAD 
    for (uint8_t i=0; i<16; i++) {
      keypad.setLED(i);
      keypad.writeDisplay();    
      delay(50);
    }
  #endif
}

void loop() {
  tree.draw();

  if (perform_zeroing_function) {
    long m = millis();
    m = m - 5000L;
    if (m > 0){
     longCountXEncoder = 0L;
     longCountYEncoder = 0L;
     longCountZEncoder = 0L;
     longCountX = 0L;
     longCountY = 0L;
     longCountZ = 0L;
     perform_zeroing_function = false;
    }  
  }

/*  if (Serial.available()) {
    byte read = Serial.read();
    switch (read) {
     case 'c': clearAndHome(); break;
     case 's': storeToolButton();break;
     case 'l': loadToolButton(); break;
     case '1': handleDigit(1); break;
     case '2': handleDigit(2); break;
     case '3': handleDigit(3); break;
     case '4': handleDigit(4); break;
     case '5': handleDigit(5); break;
     case '6': handleDigit(6); break;
     case '7': handleDigit(7); break;
     case '8': handleDigit(8); break;
     case '9' : handleDigit(9); break;

     case 'x': presetX(); break;
     case 'y': presetY(); break;
     case 'z': presetZ(); break;
     case 'd': 
               Serial.print("longCountXEncoder:");
               Serial.println(longCountXEncoder);
               Serial.print("longCountX:");
               Serial.println(longCountX);
               Serial.print("convertx:");
               Serial.println(convertx);               
               break;
    }
  }*/

  #ifdef TRELLIS_KEYPAD
    if (keypad.readSwitches()) {
      for (uint8_t i=0; i<KEYPAD_KEYSCOUNT; i++) {
        if (keypad.justPressed(i)) {
          tone(11,532,100);
          keyButtonAction[i]();  
          delay(50);
        }

      }
    }

  #endif

  newPosition = count;
  if (newPosition != oldPosition) {
    
    if (newPosition/4 - oldPosition/4 == COUNT_PER_DETENTE) {
      //Serial.println("increment");
      encoderRotated = ENCODERROTATED_CLOCKWISE;
    }

    if (oldPosition/4 - newPosition/4 == COUNT_PER_DETENTE) {
      //Serial.println("decrement");
      encoderRotated = ENCODERROTATED_ANTICLOCKWISE;
    }

    oldPosition = newPosition;
  }
}

void myuserscreen() {

  if (machine_mode == LATHE_METRIC || machine_mode == MILL_METRIC) {
    if (countingpulsesmode == false) {
      convertx = CONVERTX_METRIC;
      converty = CONVERTY_METRIC;
      convertz = CONVERTZ_METRIC;  
    }

    convertx_preset = 1.0;
    converty_preset = 1.0;
    convertz_preset = 1.0;
  }  

  else {
    if (countingpulsesmode == false) {
      convertx = CONVERTX_IMPERIAL;
      converty = CONVERTY_IMPERIAL;
      convertz = CONVERTZ_IMPERIAL;

      if (machine_mode == LATHE_METRIC || machine_mode == LATHE_IMPERIAL) {
        convertx = CONVERTZ_IMPERIAL;
        converty = CONVERTX_IMPERIAL;
      }
    }

    convertx_preset = 25.4;
    converty_preset = 25.4;
    convertz_preset = 25.4;
  }


  float presetlongx = (float) longCountX/(convertx_preset);
  float presetlongy = (float) longCountY/(converty_preset);   
  float presetlongz = (float) longCountZ/(convertz_preset);

  float encoderlongx = (float) longCountXEncoder/convertx; 
  float encoderlongy = (float) longCountYEncoder/converty; 
  float encoderlongz = (float) longCountZEncoder/convertz; 

  float longx = presetlongx + encoderlongx;
  float longy = presetlongy + encoderlongy;
  float longz = presetlongz + encoderlongz;

  int units = machine_mode == LATHE_METRIC || machine_mode == LATHE_IMPERIAL ? 0:1;

  if (machine_mode == LATHE_METRIC || machine_mode == LATHE_IMPERIAL) {
    if (lathe_mode == LATHEMODE_DIAMETER) {
      longy = longy * 2.0;
    }

    strcpy(lcdchars, "LATHE   ");
    strcat(lcdchars, METRICIMP_LABEL[units]);  
    strcat(lcdchars, DIARAD_LABEL[lathe_mode]);
    strcat(lcdchars, "\nX:");
    dtostrf(longy, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "\nZ:");
    dtostrf(longx, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "\n");
    lcd.setCursor ( 9, 3 );        // go to the next line
    lcd.print (" Ver.4.4.16 ");
  }

  else {
    strcpy(lcdchars, "MILL(");
    strcat(lcdchars, METRICIMP_LABEL[units]);
    strcat(lcdchars, ")\nX:");
    dtostrf(longx, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "\nY:");
    dtostrf(longy, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "\nZ:");
    dtostrf(longz, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars,"\n");
    lcd.setCursor ( 9, 3 );        // go to the next line
    lcd.print (" Ver.4.4.16 ");
  }
  
  tree.drawUsrScreen(lcdchars); 
}

int navigation() {

  if (encoderRotated == ENCODERROTATED_CLOCKWISE) {
    tone(11,532,100);
    encoderRotated = ENCODERROTATED_NONE;
     
      if (tree.cur_menu->parent == 0) 
      return  MW_BTU;
    else 
      return MW_BTL;
  }
  
  if (encoderRotated == ENCODERROTATED_ANTICLOCKWISE) {
    tone(11,532,100); 
    encoderRotated = ENCODERROTATED_NONE;
   
         if (tree.cur_menu->parent == 0) 
      return MW_BTD;
    else
      return MW_BTR;
  } 
    
  if (encoderClicked) {   
    encoderClicked = false;
    return MW_BTC;
  }

  if (displayUsrScreenImmediately) {
   displayUsrScreenImmediately= false;
   return MW_BTE;
  }

 /* if (pressEscapeKey) {
    pressEscapeKey = false;
    return MW_BTE;
  }

  if (pressEnterKey) {
    pressEnterKey = false;
    return MW_BTC;
  }*/

  else
     return MW_BTNULL;  
}

void zeroaxis() {
  presetX();
  presetY();
  presetZ();
  displayUsrScreenImmediately=true;
}

void reset() {
  zeroaxis();
  machine_mode = LATHE_METRIC;
  convertx = CONVERTX_METRIC;
  converty = CONVERTY_METRIC;
  convertz = CONVERTZ_METRIC;
  displayUsrScreenImmediately=true;
  countingpulsesmode = false;  

}

void countpulses() {
  convertx = 1.0;
  converty = 1.0;
  convertz = 1.0;
  displayUsrScreenImmediately=true;
  countingpulsesmode = true;
}

void exitMenu() {
  displayUsrScreenImmediately=true;  
}

void encoderButtonClicked() {
  encoderClicked = true;
   tone(11,532,100); 
  delay(40);
}

void encoderTurned() {
   //tone(11,532,100);
  uint8_t s = state & 3;
  if (digitalRead(ENCODER_A)) s |= 4;
  if (digitalRead(ENCODER_B)) s |= 8;
  switch (s) {
    case 0: case 5: case 10: case 15:
      break;
    case 1: case 7: case 8: case 14:
      count++; break;
    case 2: case 4: case 11: case 13:
      count--; break;
    case 3: case 12:
      count += 2; break;
    default:
      count -= 2; break;
  }
  state = (s >> 2);
}

void handleDigit(int digit) {
double correct_conversion;

  if (loadtool_mode == true) {
    if (machine_mode ==  LATHE_METRIC || LATHE_IMPERIAL) {
      if (touchoff == TOUCHX) {
          longCountYEncoder = tool_y[digit] + longCountYEncoder - tool_y[current_tool_selection];
      }

      else {
         longCountXEncoder = tool_x[digit] + longCountXEncoder - tool_x[current_tool_selection];
      }

      current_tool_selection = digit;
      loadtool_mode = false;
     // showToolsOffsets();
      return; 
      }

    longCountXEncoder = tool_x[digit];
    longCountYEncoder = tool_y[digit];
    loadtool_mode = false;
   // showToolsOffsets();
    return;  
  }

  else if (storetool_mode == true) {
    if (machine_mode ==  LATHE_METRIC || LATHE_IMPERIAL) {
      if (touchoff == TOUCHX) {
        if (digit == 1) {
          longCountYEncoder = valueToPulses(PART_PROBE_DIAMETER/2, converty);
          tool_y[1] = 0;
        }
        else {
            tool_y[digit] = (long) (valueToPulses(PART_PROBE_DIAMETER/2, converty) - longCountYEncoder);
        }
      }
      else {
        if (digit == 1) {
          tool_x[1] = 0;
        }
        else {
           tool_x[digit] = -longCountXEncoder;
        }
      }

    //  showToolsOffsets();
      return;
    }


    tool_x[digit] = longCountXEncoder;
    tool_y[digit] = longCountYEncoder;
    storetool_mode = false;
  //  showToolsOffsets();
    return;
  }

  if (machine_mode == LATHE_IMPERIAL || machine_mode == MILL_IMPERIAL) {
    correct_conversion = 25.4;
  }

  else {
    correct_conversion = 1.0;
  }


  float x=(float)(*ptrPresetCount)/correct_conversion;

  if (current_preset_pos < 3) {
    x = 10 * x + digit;
  }

  else {
    float q =  pow(10, 2 - current_preset_pos) * digit;
    x = x + q;
  }

  *ptrPresetCount = x * correct_conversion; 
  current_preset_pos = (current_preset_pos + 1) % 6;
}

void digit1() {
  handleDigit(1);
}

void digit2() {
  handleDigit(2);
}

void digit3() {
  handleDigit(3);
}

void digit4() {
  handleDigit(4);
}

void digit5() {
  handleDigit(5);
}

void digit6() {
  handleDigit(6);
}

void digit7() {
  handleDigit(7);
}

void digit8() {
  handleDigit(8);
}

void digit9() {
  handleDigit(9);
}

void digit0() {
  handleDigit(0);
}

void decimalPointButton() {
  current_preset_pos = 3;
}

void loadToolButton() {
  loadtool_mode = true;
//  Serial.println("Load mode selected");
//  Serial.println(" ");
}

void store_tool1() {
  tool_y[1] = longCountY;
}

void store_tool2() {
  tool_y[2] = longCountY;
}

void store_tool3() {
  tool_y[3] = longCountY;
}

void store_tool4() {
  tool_y[4] = longCountY;  
}

void load_tool1() {

}  


void load_tool2() {
  
}

void load_tool3() {
  
}

void load_tool4() {
  
}


/*void escapeButton() {
  pressEscapeKey = true;
}*/

/*void enterButton() {
  pressEnterKey = true;
}*/

void presetX() {
  storetool_mode = false;
  loadtool_mode = false;
  if (machine_mode == LATHE_IMPERIAL || machine_mode == LATHE_METRIC) {
    touchoff = TOUCHX;
    longCountYEncoder = valueToPulses(PART_PROBE_DIAMETER/2.0, converty);
    return;
  }

  ptrPresetCount = &longCountX;
  *ptrPresetCount = 0;
  longCountXEncoder = 0;
  current_preset_pos = 0;

}

void presetY() {
  storetool_mode = false;
  loadtool_mode = false;
  if (machine_mode == LATHE_METRIC || machine_mode == LATHE_IMPERIAL) {
    return;
  }

  ptrPresetCount = &longCountY;
  *ptrPresetCount = 0;
  longCountYEncoder = 0;
  current_preset_pos = 0;
  storetool_mode = false;
  loadtool_mode = false;  
}

void presetZ() {
  storetool_mode = false;
  loadtool_mode = false;
  if (machine_mode == LATHE_IMPERIAL || machine_mode == LATHE_METRIC) {
    touchoff = TOUCHZ;
    longCountXEncoder=0;
    return;
  }
  ptrPresetCount = &longCountZ;
  *ptrPresetCount = 0;
  longCountZEncoder = 0;
  current_preset_pos = 0;

}


void encoderXinterrupt() {
  if (digitalRead(ENCODERX_PINA) == digitalRead(ENCODERX_PINB)) {
    longCountXEncoder++;
  }

  else {
    longCountXEncoder--;
  }
}

void encoderYinterrupt() {
  if (digitalRead(ENCODERY_PINA) == digitalRead(ENCODERY_PINB)) {
    longCountYEncoder++;
  }

  else {
    longCountYEncoder--;
  }
}

void encoderZinterrupt() {
  if (digitalRead(ENCODERZ_PINA) == digitalRead(ENCODERZ_PINB)) {
    longCountZEncoder++;
  }

  else {
    longCountZEncoder--;
  }
}


void storeToolButton() {
  storetool_mode = true;
  
 // Serial.println("Store mode selected");
//  Serial.println(" ");
}
/*
void showToolsOffsets() {
  Serial.print(F("convertx:"));
  Serial.print(converty);
  Serial.print(F("  convertz:"));
  Serial.println(convertx);  
  Serial.print(F("X: "));
  Serial.print(pulsesToValue(longCountYEncoder, converty));
  Serial.print(F(" Z: "));
  Serial.println(pulsesToValue(longCountXEncoder, convertx));

  for (int i=1; i<=4; i++) {
    Serial.print(F("[Tool "));
    Serial.print(i);  
    Serial.print(F("] - "));
    Serial.print(F("X:"));
    Serial.print(pulsesToValue(tool_y[i], converty) );
    Serial.print(F(" Z:"));
    Serial.println(pulsesToValue(tool_x[i], convertx));
  }
  Serial.println(F("-----------------------"));
}

void clearAndHome() 
{ 
  //Serial.write(27); 
  //Serial.print("[2J"); // clear screen 
  //Serial.write(27); // ESC 
  //Serial.print("[H"); // cursor to home 
  Serial.println(F("COMMANDS AVAILABLE"));
  Serial.println(F("s: store button"));
  Serial.println(F("l: load button"));
  Serial.println(F("x: clear x"));
  Serial.println(F("y: clear y"));
  Serial.println(F("z: clear z")); 
 // Serial.println(F("d: debug mode"));   
} */

float pulsesToValue(long pulses, float conversion) {
  float value = (float) pulses/conversion; 
  return value;
}

long valueToPulses(float value, float conversion) {
  long pulses = (long) (value * conversion); 
  return pulses;
}
