void writeReflowProfile() {
  setLCDColor(255,0,0);

    // read byte 0 for version number, make sure it matches
    if (EEPROM.read(0) != EEPROM_VER) {
      for (int x=0; x < 4; x++) {
        flashLCDWarning();
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print(F("Wrong EEPROM Version"));
      }
    }

  int indexes[] = {2,10,18,26};
  for (int x=0; x < 4; x++) {
    // 2, 10, 18, 26
    int promIndex = indexes[x];
    EEPROM.write(promIndex + 0, currentReflowSettings[x].tempMin);
    delay(5);
    EEPROM.write(promIndex + 2, currentReflowSettings[x].tempMax);
    delay(5);
    EEPROM.write(promIndex + 4, currentReflowSettings[x].timeMin);
    delay(5);
    EEPROM.write(promIndex + 6, currentReflowSettings[x].timeMax);
    delay(5);
  }
  determineLCDColor();
  readReflowProfile();
  return;
}

void restoreDefaults() {
  setLCDColor(255,0,0);
  int indexes[] = {2,10,18,26};
  for (int x=0; x < 35; x++) {
    EEPROM.write(x, EEPROM.read(100+x));
  }
  readReflowProfile();
  determineLCDColor();
}

void settingsLoadTestValues() {
  // TODO, Take this out or handle it different
  // needed an easy way to switch back to test profile
  // currently doesn't save settings to EEPROM, just RAM.
  currentReflowSettings[preheat].tempMin = 0;
  currentReflowSettings[preheat].tempMax = 45;
  currentReflowSettings[preheat].timeMin = 0;
  currentReflowSettings[preheat].timeMax = 50;
  
  currentReflowSettings[soak].tempMin = 65;
  currentReflowSettings[soak].tempMax = 75;
  currentReflowSettings[soak].timeMin = 6;
  currentReflowSettings[soak].timeMax = 12;

  currentReflowSettings[reflow].tempMin = 90;
  currentReflowSettings[reflow].tempMax = 100;
  currentReflowSettings[reflow].timeMin = 6;
  currentReflowSettings[reflow].timeMax = 15;

  currentReflowSettings[cool].tempMin = 50;
  currentReflowSettings[cool].tempMax = 50;
  currentReflowSettings[cool].timeMin = 45;
  currentReflowSettings[cool].timeMax = 200; 
}


void readReflowProfile() {
  // TODO: Why did I do this?
  // Probably obsolete? Or is keeping "Versions" still useful?
  if (EEPROM.read(0) != EEPROM_VER) {
    for (int x=0; x < 4; x++) {
      flashLCDWarning();
      lcd.clear();
      lcd.setCursor(0,0);  
      lcd.print(F("Wrong EEPROM Version"));
      lcd.setCursor(0,1);
      lcd.print(F("Got: "));
      lcd.print(EEPROM.read(0));
      while(1);
    }
  }


  int indexes[] = {2,10,18,26};
  for (int x=0; x < 4; x++) {
    // 2, 10, 18, 26
    int promIndex = indexes[x];
    currentReflowSettings[x].tempMin = EEPROM.read(promIndex + 0);
    currentReflowSettings[x].tempMax = EEPROM.read(promIndex + 2);
    currentReflowSettings[x].timeMin = EEPROM.read(promIndex + 4);
    currentReflowSettings[x].timeMax = EEPROM.read(promIndex + 6);  
  }
  return;
}

// EEPROM Memory Map
// TODO: Document better and remove these comments.
/*
0: version

2: tempMin
4: tempMax
6: timeMin
8: timeMax

10:
12:
14:
16:

18:
20:
22:
24: 

26:
28:
30:
32: 
*/

/*
void initReflowSettings() {

// based on jedec lead-free profile
temperatureSettings preheatSets = {0,150,0,90};
temperatureSettings soakSets = {150,200,60,120};
temperatureSettings reflowSets = {217,260,60,150};
temperatureSettings coolSets = {100, 50, 45, 200}; 

  currentReflowSettings[preheat].tempMin = 0;
  currentReflowSettings[preheat].tempMax = 150;
  currentReflowSettings[preheat].timeMin = 0;
  currentReflowSettings[preheat].timeMax = 90;
  
  currentReflowSettings[soak].tempMin = 150;
  currentReflowSettings[soak].tempMax = 200;
  currentReflowSettings[soak].timeMin = 60;
  currentReflowSettings[soak].timeMax = 120;

  currentReflowSettings[reflow].tempMin = 217;
  currentReflowSettings[reflow].tempMax = 260;
  currentReflowSettings[reflow].timeMin = 60;
  currentReflowSettings[reflow].timeMax = 150;

  currentReflowSettings[cool].tempMin = 100;
  currentReflowSettings[cool].tempMax = 50;
  currentReflowSettings[cool].timeMin = 45;
  currentReflowSettings[cool].timeMax = 200; 

} */