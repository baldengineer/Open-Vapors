// Main Menu, Highest Level Screen
// Use "rememberHomeMenuSelectLine" to set menuSelectLine 
// in other menus to keep cursor in the right place

void displayHomeScreen() {
      printMenuLine("Current Status");
      printMenuLine("Reflow Profile");
      printMenuLine("Defaults");
      printMenuLine("Bacon");  //    printMenuLine("Manual Control");
  if (enterButtonStatus) {
    enterButtonStatus = false;
    rememberHomeMenuSelectLine = menuSelectLine;
    switch (menuSelectLine) {
      
      //current status
      case 0: 
        menuState = MENU_STATE_STATS;
      break;

      // reflow profile
      case 1:
        menuState = MENU_STATE_REFLOWPROFILE;
        //menuSelectLine = rememberHomeMenuSelectLine;
        menuSelectLine = 0;
      break;

      case 2: // Settings
        menuState = MENU_SETTING_LIST;
        menuSelectLine = 0;
      break;
      
      case 3: // Manual Control (Bacon)
        menuState = MENU_STATE_MANUAL;
        menuSelectLine = 0;
      break;
    }  
  }
}
