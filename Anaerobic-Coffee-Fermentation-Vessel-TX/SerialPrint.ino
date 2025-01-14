//PRINTING AND DEBUGGING

void printData_SurfaceTemp() {
  Serial.println("================================================");
  Serial.println("Surface Temperature Sensor:");
  dallas_sensors.requestTemperatures();

  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(dallas_sensors.getTempCByIndex(0));
  Serial.print((char)176);  //shows degrees character
  Serial.print("C  |  ");

  //print the temperature in Fahrenheit
  Serial.print((dallas_sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);  //shows degrees character
  Serial.println("F");
  Serial.println("================================================");
}

void printData_m5() {
  if (m5.readSample()) {
    Serial.println("================================================");
    Serial.println("M5 Sensor:");
    Serial.print("  RH: ");
    Serial.println(m5.getHumidity(), 2);
    Serial.print("  T:  ");
    Serial.println(m5.getTemperature(), 2);
    Serial.println("================================================");
  } else {
    Serial.println("================================================");
    Serial.println("M5 Sensor: Error in readSample()");
    Serial.println("================================================");
  }
}

void printData_AHT20() {
  Serial.print("\n\n\n");
  Serial.println("================================================");
  Serial.println("AHT20 Sensor:");
  Serial.print("Temperature: ");
  Serial.print(aht20_temp.temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(aht20_humidity.relative_humidity);
  Serial.println("% rH");
  Serial.println("================================================");
}

void printData_TDS() {
  // Serial.print("\n\n\n");
  // Serial.println("================================================");
  // Serial.println("TDS Sensor:");
  // gravityTds.setTemperature(dallas_sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
  // gravityTds.update();                                           //sampMMtdsValue, 0);
  // Serial.println("TDS Sensor:");
  // Serial.println("ppm");
  // Serial.println("================================================");

  Serial.print("\n\n\n");
  Serial.println("================================================");
  Serial.println("TDS Sensor:");                          // then get the value
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  Serial.println("================================================");
}

void printData_pH4052C() {
  Serial.print("\n\n\n");
  Serial.println("================================================");
  Serial.println("pH4052C Sensor:");
  // Read the temperature from the sensor
  //Serial.println("Temperature reading:"
  //               + String(ph4502.read_temp()));

  // Read the pH level by average
  Serial.println("pH Level Reading: "
                 + String(ph4502.read_ph_level()));

  // Read a single pH level value
  //Serial.println("pH Level Single Reading: "
  //               + String(ph4502.read_ph_level_single()));
  Serial.println("================================================");
}