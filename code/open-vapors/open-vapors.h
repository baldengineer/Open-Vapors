#include "Arduino.h"
#include "Wire.h"
#include <LiquidTWI.h>

// Button Related Macros
#define UP 0
#define RIGHT 4
#define DOWN 2
#define LEFT 3
#define ENTER 1
#define buttonBounceInterval 250
#define ACTIVE LOW
#define NOT_ACTIVE HIGH
#define PRESSED LOW
#define NOT_PRESSED HIGH

// Symbols for LCD
#define DEGREE 0xdf
#define ARROW 0x7e

#define EEPROM_VER 0x01
#define USE_SERIAL
//#define SHOW_ERROR

// LCD Color Combinations
#define LCD_OFF 0,0,0
#define LCD_ON 255,255,255
#define LCD_RED 255,0,0
#define LCD_GREEN 0,255,0
#define LCD_BLUE 0,0,255
#define LCD_ORANGE 255,128,0
#define LCD_PURPLE 255,0,255

// Beep Beep
const byte buzzer_pin = 13;
const byte normal_tone = 1000;
const byte normal_duration = 500;
const byte warning_tone = 2500;
const byte warning_duration = 250;
const byte error_tone = 3000;
const byte error_duration = 500;

// Constants for accessing EEPROM
const byte preheat = 0;
const byte soak = 1;
const byte reflow = 2;
const byte cool = 3;

// LCD backlight pins
// TODO: Move to Pins.h or something
const byte lcdRGB[] = {11,10,9};

// Pins used by front panel 
// TODO: Move to Pins.h or something
const byte buttonPins[] = {A5,A1,A4,A2,A3};

// Basic Timing Variables
unsigned long currentMillis;
unsigned long buttonPreviousMillis;
unsigned long thermalcoupleMillis;
unsigned long serialLogMillis;

const int serial_interval_idle = 1000;
const int serial_interval_active = 1000;
int serialLogInterval = serial_interval_idle;
int thermalcoupleInterval = 100;

// Types for Menu
typedef enum MENU_STATE {
  MENU_STATE_HOME,
  MENU_STATE_STATS,
  MENU_STATE_REFLOWPROFILE,
  MENU_STATE_EDIT_REFLOW,
  MENU_STATE_SETTINGS,
  MENU_STATE_MANUAL,
  MENU_STATE_EDIT_NUMBER,
  MENU_STATE_EDIT_NUMBER_DONE,
  MENU_SETTING_LIST,
  MENU_SETTINGS_DEFAULT_DONE,
  MENU_SETTING_VERIFY_DEFAULT,
}
menuState_t;

struct temperatureSettings {
  byte tempMin;
  int tempMax;
  byte timeMin;
  int timeMax;
};

// variable used when editing a number value
struct numberEditor {
  unsigned int min;
  unsigned int max;
  unsigned int current;
  byte leftpos;
  byte length;
  byte currentpos;
};


double currentTemperature=0.0;
int targetTemperature=150;

// hold overs from control code
// used by the reflow editor remoteNumber
// TODO Remove these 
const char* tempStateNames[] = {
  "Preheat",
  "Soak",
  "Reflow",
  "Cool",
  "FAIL"
};