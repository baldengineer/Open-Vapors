#include "Arduino.h"
#include "Wire.h"
#include <EEPROM.h>
//#include <LiquidCrystal.h>
#include <LiquidTWI.h>

const byte lcdRGB[] = {11,10,9};

const byte LCD_WIDTH = 20;
const byte LCD_HEIGHT = 4;
char messageBuffer[21];
byte menuPrintLine=0;

LiquidTWI lcd(0);

#define EEPROM_VER 0x01

byte promValues[] = {
	1,0,
	0,0,
	150,0,
	0,0,
	90,0,
	150,0,
	200,0,
	60,0,
	120,0,
	217,0,
	254,0,
	60,0,
	150,0,
	100,0,
	50,0,
	45,0,
	200,0};

void readButton() {
	while(digitalRead(A1));
	delay(100);
	while(!digitalRead(A1));
}

void setup() {
	lcd.begin(LCD_WIDTH,LCD_HEIGHT);
	setLCDColor(0,255,127);
	pinMode(A1, INPUT_PULLUP);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Press Enter to Review");
	readButton();

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Press Enter to begin");
	pinMode(A1, INPUT_PULLUP);
	readButton();

	lcd.clear();
	delay(100);
	setLCDColor(255,0,0);
	delay(100);

	for (int x=0; x < (sizeof(promValues) / sizeof(promValues[0])); x++) {
		EEPROM.write((x), promValues[x]);
		delay(5);
		sprintf(messageBuffer, "%d: %d - %d", x, promValues[x], EEPROM.read(x));
		//lcd.print(x); lcd.print(":"); lcd.print(promValues[x]); lcd.print(",");
		printMenuLine(messageBuffer, false);
		//delay(100);
	}


	for (int x=0; x < (sizeof(promValues) / sizeof(promValues[0])); x++) {
		EEPROM.write((100+x), promValues[x]);
		delay(5);
		sprintf(messageBuffer, "%d: %d - %d", (100+x), promValues[x], EEPROM.read(x));
		//lcd.print(x); lcd.print(":"); lcd.print(promValues[x]); lcd.print(",");
		printMenuLine(messageBuffer, false);
		//delay(100);
	}

	setLCDColor(0,255,127);
	lcd.print("Press Enter to Review");
	readButton();
	
	lcd.clear();
	lcd.setCursor(0,0);
	for (int x=0; x < (sizeof(promValues) / sizeof(promValues[0])); x++) {
		sprintf(messageBuffer, "%d: %d", x, EEPROM.read(x));
		printMenuLine(messageBuffer, true);
	}
	lcd.print("Done!");
	setLCDColor(0,255,255);
	while(1);
}

void loop() {

}

void printMenuLine(char *c, bool wait) {
  int lineWidth = LCD_WIDTH-1;
  lcd.setCursor(0,menuPrintLine);
  lcd.print(c);
  for (int x=0; x < (lineWidth-strlen(c)); x++) {
    lcd.print(" ");
  }
  menuPrintLine++;
  if (menuPrintLine > 3) {
  	menuPrintLine = 0;
  	if (wait) {
  		while(digitalRead(A1));
  		delay(100);
  		while(!digitalRead(A1));
  	}
  	else
  		delay(250);
  }
}

void setLCDColor(byte R, byte G, byte B) {
  analogWrite(lcdRGB[0], (255-R));
  analogWrite(lcdRGB[1], (255-G));
  analogWrite(lcdRGB[2], (255-B));
}