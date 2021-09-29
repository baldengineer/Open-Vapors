// Manual (Bacon) Top Level Menu
// Starts Bake or allows individual control of SSR

void displayManualList() {

	// Line 1: Start/Stop
	if (reflowState == REFLOW_STATE_IDLE)
		printMenuLine("Start Bacon!");
	else {
		printMenuLine("Stop Bacon!");
	//	menuSelectLine = 0;
	}

	// Line 2: SSR On/Off
	if (ssrState)
		printMenuLine("SSR Off");
	else
		printMenuLine("SSR On");

	// Line 3: ???
	printMenuLine("");

	// Line 4: 
	printMenuLine("Return");
	
	if (menuButtonStatus) {
		menuButtonStatus = false;
		menuState = MENU_STATE_HOME;
	}

	if (enterButtonStatus) {
		enterButtonStatus = false;
		switch (menuSelectLine) {
			case 0:
				if (reflowState == REFLOW_STATE_IDLE) {
					reflowState = REFLOW_STATE_START;
					menuState = MENU_STATE_STATS;
				} else {
					ssrOFF();
					reflowState = REFLOW_STATE_IDLE;
					menuState = MENU_STATE_STATS;
				}					
			break;

			case 1:
				if (ssrState) {
					ssrOFF();
					reflowState = REFLOW_STATE_IDLE;	
				} else {
					ssrON();	
				}
				
			break;

			case 2:
				
			break;

			case 3:
			default:
				menuSelectLine = rememberHomeMenuSelectLine;
				menuState = MENU_STATE_HOME;
			break;
		}
		
	}
}
