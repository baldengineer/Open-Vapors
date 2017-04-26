// Settings Top Level Menu

void displaySettingslist() {
	printMenuLine("Restore Defaults");
	printMenuLine("Load Test Values");
	printMenuLine("");
	printMenuLine("");

	if (enterButtonStatus) {
    	enterButtonStatus = false;
    	switch (menuSelectLine){
    		case 0:
    			menuState = MENU_SETTING_VERIFY_DEFAULT;
    		break;

    		case 1:
	    		settingsLoadTestValues();
	    		//menuState = MENU_STATE_HOME;
	    		//menuSelectLine = rememberHomeMenuSelectLine;
	    		restoreDefaultsDone();
	    	break;
    	}
    }

    if (menuButtonStatus) {
    	menuButtonStatus = false;
    	menuSelectLine = rememberHomeMenuSelectLine;
		menuState = MENU_STATE_HOME;   
    	return;
    }
}

void displaySettingsVerifyDefault() {
	printMenuLine("Are you sure?");
	printMenuLine("No");
	printMenuLine("Yes");
	printMenuLine("");

	if (enterButtonStatus) {
		enterButtonStatus = false;
		switch (menuSelectLine) {
			case 1:
				menuState = MENU_SETTING_LIST;
			break;

			case 2:
		    	menuState = MENU_SETTINGS_DEFAULT_DONE;
		    	restoreDefaults();
		    break;
		}
	}
}
void restoreDefaultsDone() {
	// MENU_SETTINGS_DEFAULT_DONE
	printMenuLine("Done");
	printMenuLine("Press Enter");
	printMenuLine("");
	printMenuLine("");

	if (enterButtonStatus) {
		enterButtonStatus = false;
		menuSelectLine = rememberHomeMenuSelectLine;
		menuState = MENU_SETTING_LIST;
	}
}

