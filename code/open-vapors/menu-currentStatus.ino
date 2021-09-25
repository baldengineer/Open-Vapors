// Status Display, no additional menus or cursor
// Display lines change based on reflow states

// Convert floats to string in a few places
char float_str[8];

// for calculating cool down ramp
unsigned long previousRampMillis;
float previousTemperatureMeasurement;

void displayStatus() {
  menuSelectLine = -1;

  // Line 1 - Status
  printReflowState();
  
  // Line 2 - Current Temp
  dtostrf(currentTemperature,4,2,float_str);
  //int tempInt = currentTemperature; // cast to int
  sprintf(messageBuffer, "Temp: %s%cC", float_str, DEGREE);
  printMenuLine(messageBuffer);
  
  // Line 3 - Setpoint
  int tempSet = setpoint;
  sprintf(messageBuffer, "Target: %d.00%cC", tempSet, DEGREE);
  printMenuLine(messageBuffer);
  
  // Line 4 - Time
  if (reflowState == REFLOW_STATE_COOL) 
    calculateRampRate();
  else {
    unsigned long currentTime;
    if (reflowState != REFLOW_STATE_IDLE)
       currentTime = (millis()-timeAtThisState) / 1000;
    else
      currentTime = 0;
    sprintf(messageBuffer,"Time Here: %lus", currentTime);

    printMenuLine(messageBuffer);
  }
  
  if (menuButtonStatus) {
    menuButtonStatus = false;
    menuSelectLine = rememberHomeMenuSelectLine;
    menuState = MENU_STATE_HOME;

  }
  if (enterButtonStatus) {
    enterButtonStatus = false;
    flashLCDWarning();
  }
  processButtons();
}

void calculateRampRate() {
  static float ramp;
  // Not my favorite calculation, but want to make sure we have an indication
  // for the cool-down happening too quickly (slower than 6degree/s)
  if (millis() - previousRampMillis >= 1000) {
    ramp = previousTemperatureMeasurement - currentTemperature;
    previousTemperatureMeasurement = currentTemperature;
    previousRampMillis = millis();
  }
  dtostrf(ramp,4,2,float_str);
  sprintf(messageBuffer, "Ramp: %s%cC/s", float_str, DEGREE);
  printMenuLine(messageBuffer);
}


void printReflowState() {
  switch (reflowState) {
    case (REFLOW_STATE_IDLE):
      char msg[21];
     // rtc_update_stamp();
      sprintf(msg, "%s (%s)", "Ready ", time_string);
//      printMenuLine("Ready");
//      printMenuLine(date_stamp.toChar());
      printMenuLine(msg);
    break;

    case (REFLOW_STATE_START):
      printMenuLine("Oven Too Hot");
    break;

    case(REFLOW_STATE_PREHEAT):
        printMenuLine("Preheat");
    break;

    case(REFLOW_STATE_SOAK):
      printMenuLine("Soak");
    break;

    case(REFLOW_STATE_REFLOW):
        printMenuLine("Reflow!");
    break;

    case(REFLOW_STATE_DWELL):
        printMenuLine("Reflow Dwell");
    break;

    case (REFLOW_STATE_COOL):
      printMenuLine("Cool Down");
    break;

    case(REFLOW_STATE_COMPLETE):
      printMenuLine("Complete");
    break;

    case(REFLOW_STATE_ERROR):
      printMenuLine("ERROR");
    break;

    default:
      printMenuLine("? wat?");
    break;
  }
}