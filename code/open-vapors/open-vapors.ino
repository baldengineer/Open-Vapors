#include "Arduino.h"
#include "Wire.h"
#include <EEPROM.h>
#include <LiquidTWI.h>
#include "reflow-ovencontroller.h"
#include "open-vapors.h"
#include "reflow-thermalcouple.h"
#include <PID_v1.h>

bool gotNaN = false;
bool thermo_abort = false;
uint8_t da_error = 0;
int8_t error_counter = 0;
int8_t max_error_count = 0;

// LiquidCrystal over I2C.
// TODO: Verify Library used, I think it was a "fast" version of the library
LiquidTWI lcd(0);
const byte LCD_WIDTH = 20;
const byte LCD_HEIGHT = 4;

// length of the display plus a null
char messageBuffer[21];

byte buttonStates[5];  // updated in processButtons()

numberEditor numberEditorState = {0,0,0,0,0,0};

temperatureSettings currentReflowSettings[4];

// *** DISPLAY Menu Related Variables
boolean menuButtonStatus = false;
boolean enterButtonStatus = false;

menuState_t menuState;
byte menuStatus = 0;
byte menuSelectLine = 0;
byte rememberHomeMenuSelectLine = 0;

byte menuPrintLine = 0;
byte reflowEditIndex = 0;


void setup() {
  // TODO: Put serial support back in
  // original idea was to support LCD or serial interface
  #ifdef USE_SERIAL
    Serial.begin(9600);
  #endif // USE_SERIAL
  lcd.begin(LCD_WIDTH,LCD_HEIGHT);
  lcd.createChar(0, degree);
  setLCDColor(0,255,0);
  menuState = MENU_STATE_HOME;

  for (int x=0; x<5; x++)
    pinMode(buttonPins[x], INPUT_PULLUP);

  pinMode(ssrEnableButton, INPUT_PULLUP);
  pinMode(ssrPin, OUTPUT);
  ssrOFF();

  readReflowProfile();
  // reflashes the EEPROM, should only be used on new board/chip
  // initReflowSettings();
}

void loop() {
  #ifdef USE_SERIAL
    processSerial();
  #endif // USE_SERIAL

  currentMillis = millis();

  if ((currentMillis - thermalcoupleMillis) >= thermalcoupleInterval) {
    updateCurrentTemp();
    if (error_counter >= 10) {
      ssrOFF();
      reflowState = REFLOW_STATE_IDLE;
      reflowStatus = REFLOW_STATUS_OFF;
      thermo_abort = true;
      Serial.println("Bailing...");
    }        
    thermalcoupleMillis = currentMillis;
  }

  checkSSRButton();
  processButtons();
  processMenu();
  processPID();
  determineLCDColor();
}

void checkSSRButton() {
  static int prevSSREnableButtonStatus;
  int currSSREnableButtonStatus = digitalRead(ssrEnableButton);

  if (prevSSREnableButtonStatus != currSSREnableButtonStatus) {
    delay(10);
    currSSREnableButtonStatus = digitalRead(ssrEnableButton);
    prevSSREnableButtonStatus = currSSREnableButtonStatus;

    // SSR Enable Button has been pressed
    if (currSSREnableButtonStatus == PRESSED) {
      // canceling the bake or manually enabling the SSR?
      if (reflowState == REFLOW_STATE_IDLE) {
        if (ssrState)
          ssrOFF();
        else 
          ssrON();
      } else {
        // cancel the bake
        ssrOFF();
        reflowState = REFLOW_STATE_IDLE;
        reflowStatus = REFLOW_STATUS_OFF;
      }
    }
  }
}

#ifdef USE_SERIAL
void processSerial() {
  if (Serial.available() <1) return;

  char incomingChar = Serial.read();
  switch (incomingChar) {
    case 'I': // Enter
    case 'i':
    enterButtonStatus = true;
    break;

    case 'M':  // Menu
    case 'm':
    menuButtonStatus = true;
    break;

    case 'J':  // Move Up
    case 'j':
    if (menuSelectLine > 0) {
      menuSelectLine--;
      } else {
        flashLCDWarning();
      }
      break;

    case 'K': // move Down
    case 'k':
    if (menuSelectLine < 3) {
      menuSelectLine++;
      } else {
        flashLCDWarning();
      }
      break;
    }
  }
  #endif USE_SERIAL








