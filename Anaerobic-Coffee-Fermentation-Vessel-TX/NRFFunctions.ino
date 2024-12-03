// void nrf_mngr() {

//   Serial.println("NRF Send Payload: ");
//   Serial.print("[Surface Temperature= ");
//   Serial.print(dallas_sensors.getTempCByIndex(0));  //Degree Celcius
//   Serial.println("]");
//   nrf_encrypt(id_SurfaceTemperature, dallas_sensors.getTempCByIndex(0));
//   delay(6);

//   Serial.print("[m5 Humidity= ");
//   Serial.print(m5.getHumidity());  //%
//   Serial.println("]");
//   nrf_encrypt(id_m5Humidity, m5.getHumidity());
//   delay(6);

//   Serial.print("[m5 Temperature= ");
//   Serial.print(m5.getTemperature());  //Degree Celcius
//   Serial.println("]");
//   nrf_encrypt(id_m5Temperature, m5.getTemperature());
//   delay(6);

//   Serial.print("[aht20 Humidity= ");
//   Serial.print(aht20_humidity.relative_humidity);  //%
//   Serial.println("]");
//   nrf_encrypt(id_aht20Humidity, aht20_humidity.relative_humidity);
//   delay(6);

//   Serial.print("[aht20 Temperature= ");
//   Serial.print(aht20_temp.temperature);  //Degree Celcius
//   Serial.println("]");
//   nrf_encrypt(id_aht20Temperature, aht20_temp.temperature);
//   delay(6);

//   gravityTds.setTemperature(dallas_sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
//   gravityTds.update();                                           //sample and calculate
//   Serial.print("[TDS Value= ");
//   Serial.print(gravityTds.getTdsValue());  //ppm
//   Serial.println("]");
//   nrf_encrypt(id_tdsValue, gravityTds.getTdsValue());
//   delay(6);

//   Serial.print("[pH= ");
//   Serial.print(ph4502.read_ph_level());  //pH
//   Serial.println("]");
//   nrf_encrypt(id_pH4052C, ph4502.read_ph_level());
//   delay(6);

//   int co2_val = co2Sensor.read();
//   Serial.print("[CO2=");
//   Serial.print(co2_val);
//   Serial.println("]");
//   nrf_encrypt(id_CO2, co2_val);
//   delay(6);

//   float oxygenData = oxygen.getOxygenData(COLLECT_NUMBER);
//   Serial.print("[O2=");
//   Serial.print(oxygenData);
//   Serial.println("]");
//   nrf_encrypt(id_O2, oxygenData);
//   delay(6);
// }


// void nrf_encrypt(uint8_t id, float dataVal) {

//   digitalWrite(LED_green, 1);
//   data.data_id = id;
//   data.paraData = dataVal;
//   nrf24.write(&data, sizeof(payload));
//   Serial.println("Package:");
//   if (data.data_id <= 0x0F) {
//     Serial.print("0x0");
//     Serial.println(data.data_id, HEX);
//   } else {
//     Serial.print("0x");
//     Serial.println(data.data_id, HEX);
//   }
//   Serial.println(data.paraData);

//   Serial.println("NRF SENT DATA");

//   digitalWrite(LED_green, 0);
// }
