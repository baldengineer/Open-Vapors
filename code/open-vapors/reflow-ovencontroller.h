

// ***** REFLOW TYPE DEFINITIONS *****
typedef enum REFLOW_STATE{
  REFLOW_STATE_IDLE,
  REFLOW_STATE_START,
  REFLOW_STATE_PREHEAT,
  REFLOW_STATE_SOAK,
  REFLOW_STATE_REFLOW,
  REFLOW_STATE_DWELL,
  REFLOW_STATE_COOL,
  REFLOW_STATE_COMPLETE,
  REFLOW_STATE_ERROR
} 
reflowState_t;

typedef enum REFLOW_STATUS{
  REFLOW_STATUS_OFF,
  REFLOW_STATUS_ON
} 
reflowStatus_t;

typedef enum DEBOUNCE_STATE{
  DEBOUNCE_STATE_IDLE,
  DEBOUNCE_STATE_CHECK,
  DEBOUNCE_STATE_RELEASE
} 
debounceState_t;

// ***** CONSTANTS *****
#define TEMPERATURE_ROOM 50
// These constants are here from original code
// most have been converted to variables and
// are stored in EEPROM
// TODO: Clean up constants
#define TEMPERATURE_SOAK_MIN 65
#define TEMPERATURE_SOAK_MAX 75
#define TEMPERATURE_REFLOW_MAX 100
#define TEMPERATURE_COOL_MIN 50
#define SENSOR_SAMPLING_TIME 250
#define SOAK_TEMPERATURE_STEP 5
#define SOAK_MICRO_PERIOD 9000
#define DEBOUNCE_PERIOD_MIN 50
#define THERMOCOUPLE_DISCONNECTED 10000

// ***** PID PARAMETERS *****
#define PID_SAMPLE_TIME 1000

// ***** PRE-HEAT STAGE *****
// TODO: Convert these to variables and store in EEPROM
// Original code values were way to aggressive
#define PID_KP_PREHEAT 25
#define PID_KI_PREHEAT 0.08
#define PID_KD_PREHEAT 10
// ***** SOAKING STAGE *****
#define PID_KP_SOAK 50
#define PID_KI_SOAK 0.05
#define PID_KD_SOAK 250
// ***** REFLOW STAGE *****
#define PID_KP_REFLOW 250
#define PID_KI_REFLOW 0.05
#define PID_KD_REFLOW 375
// ***** DWELL STAGE *****
#define PID_KP_DWELL 25
#define PID_KI_DWELL 0.025
#define PID_KD_DWELL 20

// ***** PID CONTROL VARIABLES *****
// PID Library expects doubles, even though
// arduino doesn't support them
double setpoint;
double input;
double output;
double kp = PID_KP_PREHEAT;
double ki = PID_KI_PREHEAT;
double kd = PID_KD_PREHEAT;
int windowSize = 2000;
unsigned long windowStartTime;
unsigned long nextCheck;
unsigned long nextRead;
unsigned long timerSoak;
unsigned long buzzerPeriod;
unsigned long timeAtThisState=0;

// Reflow oven controller state machine state variable
reflowState_t reflowState;
// Reflow oven controller status
reflowStatus_t reflowStatus;

// TODO: Figure out what degree symbol LCD is using
// ***** DEGREE SYMBOL FOR LCD *****
/*unsigned char degree[8]  = {
  140,146,146,140,128,128,128,128};*/

// calculated value from: https://omerk.github.io/lcdchargen/
// neither of these are used right now.
byte degree[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// Track SSR State
bool ssrState = false;

// ***** PIN ASSIGNMENT *****
// TODO: Move other Pin Assignments here (buzzer)
int ssrPin = 5;
int ssrEnableButton = 6;