#include "Wire.h"
//#include <LiquidCrystal.h>
#include <LiquidTWI.h>

LiquidTWI lcd(0);


const byte LCD_WIDTH = 20;
const byte LCD_HEIGHT = 4;

byte buttonPins[] = {
  A1,A2,A3,A4,A5};
  
void setup() {
  Serial.begin(9600);
  lcd.begin(LCD_WIDTH,LCD_HEIGHT);

  for (int x=0; x<5; x++)
    pinMode(buttonPins[x], INPUT_PULLUP);

}

void loop() {
  lcd.setCursor(0,0);
  for (int x=0; x<5; x++)
    if (!digitalRead(buttonPins[x]))
      lcd.print(buttonPins[x]);
}

/*
19 - Enter A1
20 - Left  A2
21 - Right A3
22 - Down  A4
23 - Up    A5

Left/Right?
*/

