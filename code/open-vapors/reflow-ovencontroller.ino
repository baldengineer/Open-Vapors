// Specify PID control interface
PID reflowOvenPID(&currentTemperature, &output, &setpoint, kp, ki, kd, DIRECT);

void processPID(){
  // check for state change
  if (previous_reflowState != reflowState) {
    previous_reflowState = reflowState;  // remember new state

    tone(buzzer_pin, normal_tone, normal_duration);
  }

  // Reflow oven controller state machine
  switch (reflowState) {
    case REFLOW_STATE_THERM_FAIL:
    // print failure to screen
    printMenuLine("THERMAL FAIL");
    setLCDColor(LCD_RED);

    while(1) {
      static unsigned long prev_millis = millis();
      static bool buzz_state = true;
      ssrOFF();
      if (millis() - prev_millis >= 500) {
        if (buzz_state) {
          tone(buzzer_pin, error_tone);
          buzz_state = false;  
           #ifdef USE_SERIAL
            Serial.print(F("THERMAL FAIL"));
           #endif    
        } else {
          noTone(buzzer_pin);
          buzz_state = true;
        }

        prev_millis = millis();
      }
    }

    break;

    case REFLOW_STATE_IDLE:
      //ssrOFF();
      serialLogInterval = serial_interval_idle;
      reflowStatus = REFLOW_STATUS_OFF;
    break;

    case REFLOW_STATE_START:
      ssrOFF();
        // We want oven relatively cool before starting
        setpoint = TEMPERATURE_ROOM;
        if (true) {
       // if ((currentTemperature <= TEMPERATURE_ROOM)) {
          // Initialize PID control window starting time
          windowStartTime = millis();
          // Ramp up to minimum soaking temperature
          setpoint = currentReflowSettings[soak].tempMin;
          // Tell the PID to range between 0 and the full window size
          reflowOvenPID.SetOutputLimits(0, windowSize);
          reflowOvenPID.SetSampleTime(PID_SAMPLE_TIME);
          // Turn the PID on
          reflowOvenPID.SetMode(AUTOMATIC);
          // Proceed to preheat stage
          timeAtThisState = millis();
          reflowState = REFLOW_STATE_PREHEAT;
          serialLogInterval = serial_interval_active;
        }
    break;

    case REFLOW_STATE_PREHEAT:
      reflowStatus = REFLOW_STATUS_ON;
      // If minimum soak temperature is achieve       
      if (currentTemperature >= currentReflowSettings[soak].tempMin) {
        // If we ramped too quickly, calm the ramp down
        reflowOvenPID.SetTunings(PID_KP_SOAK, PID_KI_SOAK, PID_KD_SOAK);
        // Time for soaking stage?
        if ((millis() - timeAtThisState) > (currentReflowSettings[preheat].timeMin)) {
          timeAtThisState = millis();
          reflowState = REFLOW_STATE_SOAK;   
        }
      }
    break;

    case REFLOW_STATE_SOAK:     
      // Set less agressive PID parameters for soaking ramp
      reflowOvenPID.SetTunings(PID_KP_SOAK, PID_KI_SOAK, PID_KD_SOAK);
      // Set midpoint halfway, we're going to overshoot a little
      // setpoint = ((currentReflowSettings[soak].tempMax - currentReflowSettings[soak].tempMin) / 2) + currentReflowSettings[soak].tempMin;
      setpoint = ((currentReflowSettings[soak].tempMax - currentReflowSettings[soak].tempMin)) + currentReflowSettings[soak].tempMin;
      // Make sure we sit at soak for long enough. Really helps with the reflow-peak
      if (currentTemperature > currentReflowSettings[soak].tempMin) {
        if ((millis() - timeAtThisState) > (currentReflowSettings[soak].timeMin * 1000UL)) {  
          // Proceed to reflowing state
          timeAtThisState = millis();
          reflowState = REFLOW_STATE_REFLOW; 
        }
      }
    break; 

    case REFLOW_STATE_REFLOW:
      // Set agressive PID parameters for reflow ramp
      reflowOvenPID.SetTunings(PID_KP_REFLOW, PID_KI_REFLOW, PID_KD_REFLOW);
      // Ramp to max temp.
      setpoint = currentReflowSettings[reflow].tempMax; 

      // compensating for overshoot
   /*   if (setpoint - currentTemperature <= 10.0) {
        reflowOvenPID.SetTunings(PID_KP_PREHEAT, PID_KI_PREHEAT, PID_KD_PREHEAT);
      }*/
        
      // got our min temp, will still hit the peak by interita. 
      if (currentTemperature > currentReflowSettings[reflow].tempMax) {
        // Proceed to dwell state
        timeAtThisState = millis();
        reflowState = REFLOW_STATE_DWELL; 
      }
    break;   

    case REFLOW_STATE_DWELL:
      // TODO: change timeAtThisState to include the time from when peak was hit
      // which would make settings for Reflow Max be: Reflow + Dwell
      // instead of the hardcoded 15 and 30 seconds here.
      setpoint = ((currentReflowSettings[reflow].tempMax - currentReflowSettings[reflow].tempMin) / 2) + currentReflowSettings[reflow].tempMin; 

      // just maintaining temp here, so conservative values
      reflowOvenPID.SetTunings(PID_KP_DWELL, PID_KI_DWELL, PID_KD_DWELL);
      if ((currentTemperature >= currentReflowSettings[reflow].tempMin) && (millis() - timeAtThisState >= 15000UL)) {
        reflowState = REFLOW_STATE_COOL;
        setpoint = currentReflowSettings[cool].tempMin;          
        timeAtThisState = millis();        
      }

      if (millis() - timeAtThisState >= 30000UL) {
        // spent too long here
        // TODO: Throw an Error
        reflowState = REFLOW_STATE_COOL;
        // TODO, cooling isn't working right.
        setpoint = currentReflowSettings[cool].tempMin;          
        timeAtThisState = millis();     
      }
    break;

    case REFLOW_STATE_COOL:
      // should keep heater available to achieve min cool down rate
      // but not usually an issue
      reflowStatus = REFLOW_STATUS_OFF;   
      ssrOFF();   
      // If minimum cool temperature is achieve       
      if (currentTemperature <= currentReflowSettings[cool].tempMin)
      {
        // Retrieve current time for buzzer usage
        // TODO: Renable the buzzer code
        // also, I don't like how original code used millis()
        //buzzerPeriod = millis() + 1000;

        //digitalWrite(buzzer, HIGH);
        // Turn off reflow process
        reflowStatus = REFLOW_STATUS_OFF;                
        
        // Proceed to reflow Completion state
        timeAtThisState = millis();
        reflowState = REFLOW_STATE_COMPLETE; 
      }         
      break;    

    case REFLOW_STATE_COMPLETE:
      setpoint = 0;
     // if (millis() > buzzerPeriod)
     // {
        // Turn off buzzer
        //digitalWrite(buzzer, LOW);
        // Reflow process ended
        timeAtThisState = millis();
        reflowState = REFLOW_STATE_IDLE; 
        ssrOFF();
     // }
      break;

    case REFLOW_STATE_ERROR:
      // If thermocouple is still not connected
      // the display code will turn the LCD red
      if (currentTemperature == THERMOCOUPLE_DISCONNECTED)
      {
        // Wait until thermocouple wire is connected
        reflowState = REFLOW_STATE_ERROR; 
      }
      else
      {
        // Clear to perform reflow process
        reflowState = REFLOW_STATE_IDLE; 
        ssrOFF();
      }
      break;    
  }    
  
  // PID computation and SSR control
  if (reflowStatus == REFLOW_STATUS_ON) {
    unsigned long now = millis();
    
    reflowOvenPID.Compute();

    // TODO: fix this millis() code
    if ((now - windowStartTime) > windowSize) { 
      // Time to shift the Relay Window
      windowStartTime += windowSize;
    }

    if (output > (now - windowStartTime))
      ssrON();
    else 
      ssrOFF();
  }
}


void ssrON() {
  // don't accidentally turn on the SSR until enough time has passed after boot.
  // I forgot why I added this, but I like the idea
  if (millis() < 2500) {
    flashLCDWarning();
    return;
  }

  digitalWrite(ssrPin, HIGH);
  ssrState = true;  
}

void ssrOFF() {
  digitalWrite(ssrPin, LOW);
  ssrState = false;

  heater_previous_millis = millis();
  heater_previous_temp = currentTemperature;
}






