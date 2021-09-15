#include "Arduino.h"
#include "Wire.h"
#include <EEPROM.h>
#include <LiquidTWI.h>
#include "reflow-ovencontroller.h"
#include "open-vapors.h"
#include "reflow-thermalcouple.h"
#include <PID_v1.h>
#include <Time.h>
#include "reflow-rtc-ds3231.h"

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
  // initReflowSettings(); // reflashes the EEPROM, should only be used on new board/chip
  ssrOFF();

  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    Serial.flush();
    tone(buzzer_pin, error_tone, error_duration);
    while(1);
  }


  pinMode(buzzer_pin, OUTPUT);
  tone(buzzer_pin, normal_tone, normal_duration);
}

void serial_msg_log() {
  #ifdef USE_SERIAL
    long currentTime= (millis()-timeAtThisState) / 1000;
  //  processSyncMessage();
    rtc_update_stamp();
    DateTime now = rtc.now();
    // State, Temp, Target, Time
    Serial.print(reflowState); Serial.print(F(","));
    Serial.print(currentTemperature); Serial.print(F(","));
    Serial.print(setpoint); Serial.print(F(","));
    Serial.print(currentTime); Serial.print(F(","));
    Serial.print(now.unixtime()); Serial.print(F(","));
    Serial.print(date_string); Serial.print(F(" "));
    Serial.print(time_string);
    Serial.println();
  #endif
}

void heater_watchdog() {
  if (ssrState && (millis() - heater_previous_millis >= runaway_timeout)) {

    float temperature_difference = currentTemperature - heater_previous_temp;
    if (temperature_difference < runaway_threshold) {
      // uh oh, bad things are happening
      // set state to alarm
      // shut off heater
      ssrOFF();
      reflowState = REFLOW_STATE_THERM_FAIL;
    } else {
      // the went up by threshold, so "reset" the timer
      heater_previous_temp = currentTemperature;
      heater_previous_millis = millis();
    }
  }
}

void loop() {
  currentMillis = millis();
  
  processSerial();
  heater_watchdog();

  if ((currentMillis - thermalcoupleMillis) >= thermalcoupleInterval) {
    updateCurrentTemp();
    thermalcoupleMillis = currentMillis;
  }

  if ((currentMillis - serialLogMillis) >= serialLogInterval) {
    serial_msg_log();
    serialLogMillis = currentMillis;
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


void processSerial() {
  #ifdef USE_SERIAL
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

    case 'T': // epoch update
    case 't':
      processSyncMessage();
    break;

    case '?':
   //   rtc_update_stamp();
   //   Serial.print(date_string); Serial.print(F(" "));
   //   Serial.println(time_string);
    DateTime now = rtc.now();
    Serial.println(now.timestamp()); //.toString()
    break;
    }
  #endif USE_SERIAL
  }









