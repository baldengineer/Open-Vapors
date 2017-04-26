void updateCurrentTemp() {
	currentTemperature = readThermalcouple();
	input = currentTemperature;
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