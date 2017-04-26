#include <SPI.h>
#include <Adafruit_MAX31855.h>

const int thermoCLK = 12;
const int thermoCS = 4;
const int thermoDO = 8;

// Initialize the Thermocouple
// initialize the library with the numbers of the interface pins
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

void updateCurrentTemp();

