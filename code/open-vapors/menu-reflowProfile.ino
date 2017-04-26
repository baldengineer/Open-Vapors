// Reflow Profile Top Level menu

void displayReflowProfile() {
  // MENU_STATE_REFLOWPROFILE
  for(int x=0; x<4; x++) {
    //sprintf(messageBuffer, "%s: %dC,%ds", tempStateNames[x],reflowStageTemps[x], reflowStageTimes[x]);     
    sprintf(messageBuffer, "%s: %d%cC,%ds", tempStateNames[x],currentReflowSettings[x].tempMax, DEGREE, currentReflowSettings[x].timeMax);
    printMenuLine(messageBuffer);
  }

  if (menuButtonStatus) {
    menuButtonStatus = false;
    menuSelectLine = rememberHomeMenuSelectLine;
    menuState = MENU_STATE_HOME;
  }

  if (enterButtonStatus) {
    enterButtonStatus = false;
    menuState = MENU_STATE_EDIT_REFLOW;
    reflowEditIndex = menuSelectLine;
  }
}

void doneEditingReflowNumbers() {
  menuState = MENU_STATE_EDIT_REFLOW;
  //currentReflowSettings.preheatSets.tempMax = numberEditorState.current;
  switch (menuSelectLine) {
    case 0: // tempMin
     currentReflowSettings[reflowEditIndex].tempMin = numberEditorState.current;
    break;
    case 1: // tempMax
     currentReflowSettings[reflowEditIndex].tempMax = numberEditorState.current;
    break;
    case 2: // timeMin
     currentReflowSettings[reflowEditIndex].timeMin = numberEditorState.current;
    break;
    case 3: // timeMax
     currentReflowSettings[reflowEditIndex].timeMax = numberEditorState.current;
    break;
  }
  writeReflowProfile();
}

void editReflowProfile() {
  // MENU_STATE_EDIT_REFLOW;
  // Called from MENU_STATE_REFLOWPROFILE
  //sprintf(messageBuffer, "Temp: %d", reflowStageTemps[reflowEditIndex]);
  sprintf(messageBuffer, "Min Temp: %03d%cC", currentReflowSettings[reflowEditIndex].tempMin, DEGREE);
  printMenuLine(messageBuffer);

  sprintf(messageBuffer, "Max Temp: %03d%cC", currentReflowSettings[reflowEditIndex].tempMax, DEGREE);
  printMenuLine(messageBuffer);

 // sprintf(messageBuffer, "Time: %d", reflowStageTimes[reflowEditIndex]);
  sprintf(messageBuffer, "Min Time: %03dsecs", currentReflowSettings[reflowEditIndex].timeMin);
  printMenuLine(messageBuffer);

  sprintf(messageBuffer, "Max Time: %03dsecs", currentReflowSettings[reflowEditIndex].timeMax);
  printMenuLine(messageBuffer);

  if (menuButtonStatus) {
    menuButtonStatus = false;
    menuSelectLine = rememberHomeMenuSelectLine;
    menuState = MENU_STATE_REFLOWPROFILE;
  }

  if (enterButtonStatus) {
    enterButtonStatus = false;
    menuState = MENU_STATE_EDIT_NUMBER;
    numberEditorState.min = 0;
    numberEditorState.max = 300;
    switch (menuSelectLine) {
      case 0: // Preheat
       numberEditorState.current = currentReflowSettings[reflowEditIndex].tempMin;
      break;
      case 1: // Soak
       numberEditorState.current = currentReflowSettings[reflowEditIndex].tempMax;
      break;
      case 2: // Reflow
       numberEditorState.current = currentReflowSettings[reflowEditIndex].timeMin;
      break;
      case 3: // Cool
       numberEditorState.current = currentReflowSettings[reflowEditIndex].timeMax;
      break;
    }
    numberEditorState.leftpos = 11;
    numberEditorState.length = 3;
    numberEditorState.currentpos = 11;
  }
}