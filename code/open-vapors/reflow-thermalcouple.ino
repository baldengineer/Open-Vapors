void updateCurrentTemp() {
	currentTemperature = readThermalcouple();
	input = currentTemperature;
}

float readThermalcouple() {
	static float last_reading = 0.00;
	float c = thermocouple.readCelsius();
	if (c == THERMOCOUPLE_DISCONNECTED) {
      reflowState = REFLOW_STATE_ERROR;
      reflowStatus = REFLOW_STATUS_OFF;
	}

	if (isnan(c)) {
		gotNaN = true;
		da_error = thermocouple.readError();
		error_counter++;
		if (error_counter >= max_error_count)
			max_error_count = error_counter;
		Serial.println(F("NaN"));
		return last_reading;
	}
//	error_counter = 0;
	if (c == 0.00) {
		Serial.println(F("dbg: 0.00"));
	}
	if (error_counter > 0)
		error_counter--;
	last_reading = c;
	return c;
}