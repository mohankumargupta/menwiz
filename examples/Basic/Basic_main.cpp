#include "main.h"

#ifdef LCD_PARALLEL
  LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
#else
  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif

#ifdef TRELLIS_KEYPAD
  Adafruit_Trellis _keypad = Adafruit_Trellis();
  Adafruit_TrellisSet keypad =  Adafruit_TrellisSet(&_keypad);
  //void (*keyButtonAction[KEYPAD_KEYSCOUNT])() = {setLatheMode, setMillMode, setMetric, setImperial, presetX, digit1, digit2, digit3, presetY, digit4, digit5, digit6,presetZ,zeroaxis,escapeButton,enterButton};
  void (*keyButtonAction[KEYPAD_KEYSCOUNT])() = {presetX, digit1, digit2, digit3, presetY, digit4, digit5, digit6,presetZ,digit7, digit8, digit9,loadToolButton,digit0,decimalPointButton,storeToolButton};
#endif

volatile int count=0;
volatile int state;
long oldPosition = -999;
long newPosition = 0;

const float CONVERT_UNITS[] = {CONVERT_METRIC, CONVERT_IMPERIAL, CONVERT_SOMETHING, CONVERT_SOMETHING2, CONVERT_SOMETHING3};
const char *MSG_UNITS[] = {"METRIC","IMPERIAL","SOMETHING1","SOMETHING2","SOMETHING3"};
int units = 0;

int lathe_mill = LATHE; //LATHE chosen by default

bool displayUsrScreenImmediately = false;
char lcdchars[80];
char buf[20];

bool pressEnterKey = false, pressEscapeKey = false;

bool encoderClicked = false;
volatile int encoderRotated = ENCODERROTATED_NONE;

menwiz tree;
volatile int countX=0, countY=0, countZ=0;
volatile long longCountX=0L, longCountY=0L, longCountZ=0L;
volatile long longCountXEncoder=0L, longCountYEncoder=0L, longCountZEncoder=0L; 
volatile long *ptrPresetCount = &longCountX;
volatile float x,y,z;
int current_preset = CONST_PRESET_X;
int current_preset_pos = 0;
float preset_x=-0.5, preset_y=-0.5, preset_z=-0.5; 
_menu *r,*s0,*s1,*s2, *s3, *s4, *q4, *p1,*p2,*p3,*p4;
int machine_mode = LATHE_METRIC;
int lathe_mode = LATHEMODE_DIAMETER;
bool loadtool_mode = false;
bool storetool_mode = false;
long tool_y[10];


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

  Serial.begin(115200); 
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
      
  tree.addUsrScreen(myuserscreen,TIMEOUT_DELAY);
  tree.addUsrNav(navigation,6);
  tree.showUsrScreen();

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

  #ifdef TRELLIS_KEYPAD
    if (keypad.readSwitches()) {
      for (uint8_t i=0; i<KEYPAD_KEYSCOUNT; i++) {
        if (keypad.justPressed(i)) {
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
  
  float presetlongx = longCountX/(CONVERT_UNITS[units] * STORE_MULTIPLE);
  float presetlongy = longCountY/(CONVERT_UNITS[units] * STORE_MULTIPLE);   
  float presetlongz = longCountZ/(CONVERT_UNITS[units] * STORE_MULTIPLE);

  //float longx = longCountX/(CONVERT_UNITS[units] * STORE_MULTIPLE);
  //float longy = longCountY/(CONVERT_UNITS[units] * STORE_MULTIPLE);
  //float longz = longCountZ/(CONVERT_UNITS[units] * STORE_MULTIPLE);

  float encoderlongx = longCountXEncoder/CONVERT_UNITS[units]; 
  float encoderlongy = longCountYEncoder/CONVERT_UNITS[units]; 
  float encoderlongz = longCountZEncoder/CONVERT_UNITS[units]; 

  longx = presetlongx + encoderlongx;
  longy = presetlongy + encoderlongy;
  longz = presetlongz + encoderlongz;

  switch(machine_mode) {
    case LATHE_METRIC:
      lathe_mill = LATHE;
      units = METRIC;
      break;
    case LATHE_IMPERIAL:
      lathe_mill = LATHE;
      units = IMPERIAL;
      break;
    case MILL_METRIC:
      lathe_mill = MILL;
      units = METRIC;
      break;
    case MILL_IMPERIAL:
      lathe_mill = MILL;
      units = IMPERIAL;
      break;
  }

  if (lathe_mill == LATHE) {
    strcpy(lcdchars, "LATHE(");
    strcat(lcdchars, MSG_UNITS[units]);
    strcat(lcdchars, ")\nX:");
    dtostrf(longx, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "\nY:");
    dtostrf(longy, 7,3,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "\n");
  }

  else {
    strcpy(lcdchars, "MILL(");
    strcat(lcdchars, MSG_UNITS[units]);
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
  }
  
  tree.drawUsrScreen(lcdchars); 
}

int navigation() {
  if (encoderRotated == ENCODERROTATED_CLOCKWISE) {
    encoderRotated = ENCODERROTATED_NONE;

    if (tree.cur_menu->parent == 0) 
      return  MW_BTU;
    else 
      return MW_BTL;
  }
  
  if (encoderRotated == ENCODERROTATED_ANTICLOCKWISE) { 
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

  if (pressEscapeKey) {
    pressEscapeKey = false;
    return MW_BTE;
  }

  if (pressEnterKey) {
    pressEnterKey = false;
    return MW_BTC;
  }

  if (Serial.available()) {
    byte read = Serial.read();
    switch (read) {
      case 'e': return MW_BTC;
      case 'u': return MW_BTU;
      case 'd': return MW_BTD;
      case 'l': return MW_BTL;
      case 'r': return MW_BTR;
      case 's': return MW_BTE;
    }
   }

  else if (preset_x + preset_y + preset_z + 1.5 > 0.01 && tree.cur_menu->parent == 0) {
    if (preset_x - 0.5 > 0.01) {
      countX = round(preset_x * CONVERT_UNITS[units] );
      longCountX = 10 * countX; 
    }
 
    else if (preset_y - 0.5 > 0.01) {
      countY = round(preset_y * CONVERT_UNITS[units] );
      longCountY = 10 * countY; 
    }

    else if (preset_z  - 0.5 > 0.01) {
      countZ = round(preset_z * CONVERT_UNITS[units] );
      longCountZ = 10 * countZ; 
    }

    preset_x = -0.5;
    preset_y = -0.5;
    preset_z = -0.5;
    return MW_BTE;
  }

  else
     return MW_BTNULL;  
}

void zeroaxis() {
  countX = 0;
  countY = 0;
  countZ = 0;
  longCountX = 0;
  longCountY = 0;
  longCountZ = 0;
  displayUsrScreenImmediately=true;
  pressEnterKey = true;
}

void exitMenu() {
  displayUsrScreenImmediately=true;  
}

void encoderButtonClicked() {
  encoderClicked = true; 
  delay(40);
}

void encoderTurned() {
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

void setLatheMode() {
  lathe_mill = LATHE;
}

void setMillMode() {
  lathe_mill = MILL;
  //Serial.println("MILL");
}

void setMetric() {
  units = METRIC;
}

void setImperial() {
  units = IMPERIAL;
}

void doNothing() {

}


void handleDigit(int digit) {

  if (loadtool_mode == true) {
    longCountY = tool_y[digit];
    loadtool_mode = false;
    return;  
  }

  else if (storetool_mode == true) {
    tool_y[digit] = longCountY;
    storetool_mode = false;
    return;
  }

  float x=(*ptrPresetCount)/(CONVERT_UNITS[units] * STORE_MULTIPLE_FLOAT);
  Serial.print("old x:");
  Serial.println(x);
  
  if (current_preset_pos < 3) {
    x = 10 * x + digit;
  }

  else {
    x = x + (float) pow(10, 2 - current_preset_pos) * digit;
  }


  Serial.print("new x:");
  Serial.println(x);
  //float increment = (float) pow(10, 2 - current_preset_pos) * digit;
  //x = x + increment;
  *ptrPresetCount = x * (CONVERT_UNITS[units] * STORE_MULTIPLE);
  
  //Serial.print("increment:");
  //Serial.println(increment);
  //*ptrPresetCount = round(x* (CONVERT_UNITS[units] * 10) + 1);
  Serial.print("preset:");
  Serial.println(*ptrPresetCount);
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


void escapeButton() {
  pressEscapeKey = true;
}

void enterButton() {
  pressEnterKey = true;
}

void presetX() {
  ptrPresetCount = &longCountX;
  *ptrPresetCount = 0;
  longCountXEncoder = 0;
  current_preset_pos = 0;
  storetool_mode = false;
  loadtool_mode = false;
}

void presetY() {
  ptrPresetCount = &longCountY;
  *ptrPresetCount = 0;
  longCountYEncoder = 0;
  current_preset_pos = 0;
  storetool_mode = false;
  loadtool_mode = false;  
}

void presetZ() {
  ptrPresetCount = &longCountZ;
  *ptrPresetCount = 0;
  longCountZEncoder = 0;
  current_preset_pos = 0;
  storetool_mode = false;
  loadtool_mode = false;  
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
}
