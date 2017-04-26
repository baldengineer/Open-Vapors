
#include "Arduino.h"
#include "Wire.h"
#include <EEPROM.h>
//#include <LiquidCrystal.h>
#include <LiquidTWI.h>

#define EEPROM_VER 0x01

const byte lcdRGB[] = {11,10,9};

const byte LCD_WIDTH = 20;
const byte LCD_HEIGHT = 4;

byte menuPrintLine = 0;
byte menuSelectLine = 0;
char messageBuffer[21];

LiquidTWI lcd(0);



void setup() {
	lcd.begin(LCD_WIDTH,LCD_HEIGHT);
  pinMode(A1, INPUT_PULLUP);
	setLCDColor(0,255,127);
	for(int x=0; x < 35; x++) {
		sprintf(messageBuffer, "%d: %d", x, EEPROM.read(x));
		printMenuLine(messageBuffer);
	}
	lcd.print("...");
  for(int x=0; x < 35; x++) {
    sprintf(messageBuffer, "%d: %d", (100+x), EEPROM.read((100+x)));
    printMenuLine(messageBuffer);
  }
	while(1);
}

void loop() {}

void printMenuLine(char *c) {
  int lineWidth = LCD_WIDTH-1;
  lcd.setCursor(0,menuPrintLine);
  lcd.print(c);
  for (int x=0; x < (lineWidth-strlen(c)); x++) {
    lcd.print(" ");
  }
  menuPrintLine++;
  if (menuPrintLine > 3) {
  	menuPrintLine = 0;
  	while(digitalRead(A1));
  	delay(100);
    while(!digitalRead(A1));
  }
}

void setLCDColor(byte R, byte G, byte B) {
  analogWrite(lcdRGB[0], (255-R));
  analogWrite(lcdRGB[1], (255-G));
  analogWrite(lcdRGB[2], (255-B));
}