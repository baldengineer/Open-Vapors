// Functions common to all of the menu levels
// and display controls

void processMenu() {
    switch (menuState) {
      case MENU_STATE_HOME:
        displayHomeScreen();
      break;

      case MENU_STATE_MANUAL:
        displayManualList();
      break; 

      case MENU_STATE_REFLOWPROFILE:
        displayReflowProfile();
      break;

      case  MENU_STATE_EDIT_REFLOW:
       // Called from MENU_STATE_REFLOWPROFILE
        editReflowProfile();
      break;

      case MENU_STATE_STATS: 
       displayStatus();
      break;

      case MENU_STATE_EDIT_NUMBER:
        editNumber();
      break;

      case MENU_STATE_EDIT_NUMBER_DONE:
        doneEditingReflowNumbers();
      break;

      case MENU_SETTING_LIST:
        displaySettingslist();
      break;

      case MENU_SETTINGS_DEFAULT_DONE: 
        restoreDefaultsDone();
      break;

      case MENU_SETTING_VERIFY_DEFAULT:
        displaySettingsVerifyDefault();
      break;
    }
  }

void printMenuLine(char *c) {
  int lineWidth = LCD_WIDTH-1;
  lcd.setCursor(0,menuPrintLine);
  if (menuPrintLine == menuSelectLine) {
    lcd.print((char)ARROW);
    lineWidth = lineWidth - 1; 
    } else {
      lcd.print(F(" "));
      lineWidth = lineWidth - 1; 
    }
    lcd.print(c);
    for (int x=0; x < (lineWidth-strlen(c)); x++) {
      lcd.print(F(" "));
    }
    menuPrintLine++;
    if (menuPrintLine > 3) menuPrintLine = 0;
    processButtons();

    if (ssrState) {
      lcd.setCursor(19,0);
      lcd.print(F("!"));
    } else {
      lcd.setCursor(19,0);
      lcd.print(F("."));
    }
}

void processButtons() {
  currentMillis = millis();
  if ((currentMillis - buttonPreviousMillis) >= buttonBounceInterval) {
    for (int x=0; x<5; x++) 
      buttonStates[x] = digitalRead(buttonPins[x]);
        
    if (buttonStates[ENTER] == LOW)
      enterButtonStatus = true;

    if (buttonStates[LEFT] == LOW)
      menuButtonStatus = true;

    if (menuState != MENU_STATE_EDIT_NUMBER) {
      if (buttonStates[DOWN] == LOW)
        if (menuSelectLine < 3)
          menuSelectLine++;
        else
          flashLCDWarning();


      if (buttonStates[UP] == LOW) {
        if (menuSelectLine > 0) {
          menuSelectLine--;
        }      
        else {
          flashLCDWarning();
        }
      }
    }   
    buttonPreviousMillis = currentMillis;
  }
}

void setLCDColor(byte R, byte G, byte B) {
  // Created some #defines for color
  // combinations
  // this function really should only be called
  // from determineLCDColor
  analogWrite(lcdRGB[0], (255-R));
  analogWrite(lcdRGB[1], (255-G));
  analogWrite(lcdRGB[2], (255-B));
}

void determineLCDColor() {
  switch (reflowState) {
    case REFLOW_STATE_COOL:
      setLCDColor(LCD_BLUE);
    break;

    case REFLOW_STATE_REFLOW:
    case REFLOW_STATE_DWELL:
      setLCDColor(LCD_PURPLE);
    break;

    case REFLOW_STATE_IDLE:
      setLCDColor(LCD_GREEN);
    break;

    case REFLOW_STATE_COMPLETE:
      setLCDColor(LCD_OFF);
    break;

    case REFLOW_STATE_ERROR:
      setLCDColor(LCD_RED);
    break;

    default:
      setLCDColor(LCD_ORANGE);
    break;     
  }
}

void flashLCDWarning() {
  setLCDColor(LCD_ORANGE);
  delay(250);
  determineLCDColor();
}
