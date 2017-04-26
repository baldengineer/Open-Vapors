// Reflow Editing Value Screen

 void editNumber() {   
    #ifdef USE_SERIAL
      Serial.println(F("Edit Not Support in Serial"));
    #endif

    lcd.cursor();
    lcd.blink();
    lcd.setCursor(numberEditorState.currentpos, menuSelectLine);

    if (buttonStates[DOWN] == ACTIVE) {
      buttonStates[DOWN] = NOT_ACTIVE;
      byte index = (numberEditorState.leftpos + numberEditorState.length) - numberEditorState.currentpos-1;  // which digit are we on
      byte currentDigit = (numberEditorState.current % mypow(index+1)) / mypow(index);  // the digit we are going to edit
      
      if (currentDigit > 0) { // it can only be between 0 and 9
        currentDigit--;  
        numberEditorState.current -= mypow(index);  // reduce by "1" order of magnitude
      } else {
        currentDigit = 9;
        numberEditorState.current += (9*mypow(index));  // we were at 0 already, roll over
      }
      lcd.noCursor();  // prevent cursor flicker
      lcd.noBlink();
      lcd.print(currentDigit);
    }

    if (buttonStates[UP] == ACTIVE) {
      buttonStates[UP] = NOT_ACTIVE;
      byte index = (numberEditorState.leftpos + numberEditorState.length) - numberEditorState.currentpos-1;  // which digit are we on
      byte currentDigit = (numberEditorState.current % mypow(index+1)) / mypow(index);  // the digit we are going to edit
      currentDigit++;

      if (currentDigit > 9) {
        currentDigit = 0;
        numberEditorState.current -= (9 * mypow(index));
      } else {
        numberEditorState.current += mypow(index);  
      }
      lcd.noCursor(); // prevent cursor flicker
      lcd.noBlink();
      lcd.print(currentDigit);
    }

    if (buttonStates[LEFT] == ACTIVE){
      buttonStates[LEFT] = NOT_ACTIVE;
      menuButtonStatus = false;
      numberEditorState.currentpos--;
      if (numberEditorState.currentpos < numberEditorState.leftpos) {
        numberEditorState.currentpos = numberEditorState.leftpos;
        flashLCDWarning();
      }
    }

    if (buttonStates[RIGHT] == ACTIVE){
      buttonStates[RIGHT] = NOT_ACTIVE;
      numberEditorState.currentpos++;
      if (numberEditorState.currentpos > (numberEditorState.leftpos + numberEditorState.length - 1)) {
        numberEditorState.currentpos = numberEditorState.leftpos + numberEditorState.length - 1;
        flashLCDWarning();
      }
    }

    if (enterButtonStatus) {
      enterButtonStatus = false;
      if (numberEditorState.current > numberEditorState.max) {
        flashLCDWarning();
        numberEditorState.current = 255;
      }
      lcd.noCursor();
      lcd.noBlink();
      menuState = MENU_STATE_EDIT_NUMBER_DONE;
  }
}

long mypow(int exp) {
  // Lookup table to get 10s values for number editing
    switch (exp){
      case 0: return 1;
      case 1: return 10;
      case 2: return 100;
      case 3: return 1000;
      case 4: return 10000;
    }
}