

bool init_ble_module() {
  // try to see if the module is there and get its
  // address, if we can.

  Serial1.setTimeout(250); // we're going to be waiting for OKs

  ble_expect_OK("AT");
  ble_expect_OK("AT+NAMEvaporsLE");
  ble_address = ble_return_str("AT+ADDR");
  if (ble_address.startsWith("+ADDR")) {
    ble_address = ble_address.substring(ble_address.indexOf("+"));
  } else {
    ble_address = "";
  }

  return true; // because we have to, for now.
}


String ble_send(const char * command) {
  Serial.print(F("Sending: "));
  Serial.println(command);
  Serial1.println(command);

  return Serial1.readString();
}

String ble_return_str (const char * command) {
  String ble_response = ble_send(command);
  if (ble_response.length() > 0) {
    Serial.print(F(">"));
    Serial.print(ble_response);
    return (ble_response);
  }
  return "";
}

bool ble_expect_OK(const char * command) {
  String ble_response = ble_send(command);
  if (ble_response.length() > 0) {
    Serial.print(F(">"));
    Serial.print(ble_response);
    return ble_response.startsWith("OK");
  }
  return false;
}

bool handle_ble_state() {
  if (ble_available) {
    static bool previous_BT_state = false;
    bool current_BT_state = digitalRead(ble_state_pin);
    if (previous_BT_state != current_BT_state) {
      if (current_BT_state)
        Serial.println("BLE Connected");
      else
        Serial.println("Lost BLE Connection.");
      previous_BT_state = current_BT_state;
    }
    return current_BT_state;
  } else {
    return false;
  }
}

void send_ble_log() {
  ble_state = handle_ble_state();
  if (ble_state) {
	  Serial1.print("NYI,"); 
	  Serial1.print(currentTemperature, 0); Serial1.print(",");
	  Serial1.println((0x0000FFFF & millis()));
  }
}
