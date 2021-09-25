
// BLE Module Pins
const byte ble_enable_pin = 7; // really used
const byte ble_state_pin = A0;

// BLE Globals
bool ble_available = true;
bool ble_state = false;
String ble_address;