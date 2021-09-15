void updateCurrentTemp() {
	double currentReading = readThermalcouple();
	if (currentReading == 0.00) {
		#ifdef USE_SERIAL
			#ifdef SHOW_ERROR
				Serial.print(F("error,"));
				Serial.println(millis());
			#endif
		#endif
		currentTemperature = currentTemperature;
	} else {
		currentTemperature = currentReading; // don't change stored ftemperature	

	}
}

float readThermalcouple() {
	float c = thermocouple.readCelsius();
	if (c == THERMOCOUPLE_DISCONNECTED) {
      reflowState = REFLOW_STATE_ERROR;
      reflowStatus = REFLOW_STATUS_OFF;
	}
	if (isnan(c))
		return 0;
	return c;
}