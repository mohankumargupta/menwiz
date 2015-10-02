#include "main.h"

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const char *MSG_UNITS[] = {"METRIC","IMPERIAL","SOMETHING1","SOMETHING2","SOMETHING3"};
int lathe_mill = LATHE; //LATHE chosen by default

bool displayUsrScreenImmediately = false;
char lcdchars[80];
char buf[20];
int units = 0;
menwiz tree;
volatile int countX=100, countY=100, countZ=100;
volatile float x,y,z;
volatile float preset_x=-1.0, preset_y=-1.0, preset_z=-1.0; 
_menu *r,*s0,*s1,*s2, *s3, *s4, *q4, *p1,*p2,*p3,*p4;


void setup() {
  
  Serial.begin(115200);    
  tree.begin(&lcd,20,4); //declare lcd object and screen size to menwiz lib
  lcd.clear();
  strcpy(lcdchars, "");

  r = tree.addMenu(MW_ROOT,NULL,F("MAIN MENU")); 
      s0 = tree.addMenu(MW_VAR, r, F("Info Screen"));
           s0->addVar(MW_ACTION, exitMenu);
           s0->setBehaviour(MW_ACTION_CONFIRM, false);           
      s1 = tree.addMenu(MW_VAR, r, F("Zero Axis"));
           s1->addVar(MW_ACTION, zeroaxis);
           s1->setBehaviour(MW_ACTION_CONFIRM, false);
      s2 = tree.addMenu(MW_SUBMENU, r, F("Preset XYZ"));
           p1 = tree.addMenu(MW_VAR, s2, F("Back to Menu") );
           p2 = tree.addMenu(MW_VAR, s2, F("Preset X"));
           p3 = tree.addMenu(MW_VAR, s2, F("Preset Y"));
           p4 = tree.addMenu(MW_VAR, s2, F("Preset Z"));
      s3 = tree.addMenu(MW_VAR, r, F("Change Units"));
           s3->addVar(MW_LIST, &units);
           s3->addItem(MW_LIST, F("METRIC"));
           s3->addItem(MW_LIST, F("IMPERIAL"));
           s3->addItem(MW_LIST, F("SOMETHING1"));
           s3->addItem(MW_LIST, F("SOMETHING2"));
           s3->addItem(MW_LIST, F("SOMETHING3"));
      s4 = tree.addMenu(MW_VAR, r, F("Lathe or Mill"));;
           s4->addVar(MW_LIST, &lathe_mill);
           s4->addItem(MW_LIST, F("Lathe"));
           s4->addItem(MW_LIST, F("Mill"));

  tree.addUsrScreen(myuserscreen,TIMEOUT_DELAY);
  tree.addUsrNav(navigation,6);
  tree.showUsrScreen();
}

void loop() {
  tree.draw();
}

void myuserscreen() {
  float x=countX/1.0,y=countY/1.0,z=countZ/1.0;
  //Serial.println("myuserscreen:");
  if (lathe_mill == LATHE) {
    strcpy(lcdchars, "LATHE(");
    strcat(lcdchars, MSG_UNITS[units]);
    strcat(lcdchars, ")\n\nX         Y\n");
    dtostrf(x, 1,2,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "    ");
    dtostrf(y, 1,2,   buf);
    strcat(lcdchars,buf);
  }

  else {
    strcpy(lcdchars, "MILL(");
    strcat(lcdchars, MSG_UNITS[units]);
    strcat(lcdchars, ")\n\nX      Y      Z\n");
    dtostrf(x, 1,1,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "  ");
    dtostrf(y, 1,1,   buf);
    strcat(lcdchars,buf);
    strcat(lcdchars, "  ");
    dtostrf(z, 1,1,   buf);
    strcat(lcdchars,buf);
  }
  
  tree.drawUsrScreen(lcdchars); 
}

int navigation() {

  if (displayUsrScreenImmediately) {
   displayUsrScreenImmediately= false;
   return MW_BTE;
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

  else
     return MW_BTNULL;  
}

void zeroaxis() {
  countX = 0;
  countY = 0;
  countZ = 0;
  displayUsrScreenImmediately=true;
}

void exitMenu() {
  displayUsrScreenImmediately=true;  
}
