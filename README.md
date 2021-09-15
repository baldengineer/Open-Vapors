# Open Vapors - Open Source Reflow Oven Controller

Additional Information on [baldengineer.com]( https://www.baldengineer.com/?s=open+vapors).

Key hardware pieces you'll need:
+ Toaster Oven with "Always On"
+ Custom controller board (or maybe an Arduino)
+ SSR (Recommend Crydom D2425)
+ 4-Line RGB LCD Display
+ 5 Button Keypad
+ 1 Push Button
+ Piezo Buzzer 

## /code
Contains all of the code for the menu display and reflow controller.

## /KiCad
The revision 2 board is a significant redesign from the first board. It integrates the shift register for the LCD and implements voltage rails to massively reduce internal wiring. Headers were also position to logical places on the PCB.

Buzzer is optional, but now implemented. USB connection through a 0.1" (2.54mm) header to a USB-port breakout. It is only intended for serial operation.

Recommend using ISP for programming. 

Also added port for AT-09 BLE module. (Not yet implemented)



## /enclosure
Laser cut enclosure. The top, bottom, and sides used 3mm black acrylic. The front and back panels are Black on White acrylic.

Originally designed in Autocad Inventor, student edition. Then exported to Adobe Illustrator (CS6 I think) files.

The bottom needs to be redesigned to get more support. ~~The SSR warps it over time.~~ After almost 5 years, it hasn't sagged anymore. I would still recommend a support around the SSR though.


## /original electronics - EAGLE
Rev 1 was created using an EAGLE. It worked okay, but relied on a "LCD Backpack" and was not an optimized design. (LEDs in the wrong place.) 

That said, it worked great for years.

The original keypad is in use still. Even though the debounce caps were totally wrong.
