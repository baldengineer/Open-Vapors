Open Vapors - Open Source Reflow Oven Controller
More Information: https://www.baldengineer.com/?s=open+vapors

Complete project files.

Key hardware pieces you'll need:
+ Toaster Oven with "Always On"
+ 328p or 32u4 based Arduino Board (or custom controller board)
+ SSR (Recommend Crydom D2425)
+ 4-Line RGB LCD Display
+ 5 Button Keypad
+ 1 Push Button
+ Optional: Buzzer (not yet implemented)

/code
Contains all of the code for the menu display and reflow controller.


/controller - EAGLE
Rev 1 (the only one I've built) and Rev 2 (with some changes).
It is a AVR 32u4 (Arduino Leonardo) based board. Key features include
some LCD Status lights, function specific headers, and a USB Port.

USB port was originally intended to be for re-programming firmware and for data logging.


/ enclosure
The enclosure I made is laser cut. The top, bottom, and sides used 3mm black acrylic. The front and back panels are Black on White acrylic.

Originally designed in Autocad Inventor, student edition. Then exported to Adobe Illustrator (CS6 I think) files.

The bottom needs to be redesigned to get more support. The SSR warps it over time.


/keyapd - EAGLE
Rev 1 of the keypad is so bad I won't post it. Just a basic board for the 5 buttons.