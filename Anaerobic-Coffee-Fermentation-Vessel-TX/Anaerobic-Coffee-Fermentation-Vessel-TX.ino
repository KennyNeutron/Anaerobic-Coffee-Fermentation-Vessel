/*
Development of Anaerobic Coffee Fermentation Vessel

MAIN CODE

Environment: ATMEGA328P-AU

by: Kenny Neutron

Date Started: 12-23-2023
Date Finished:

*/
#include <Wire.h>
#include "SHTSensor.h"
#include <Adafruit_AHTX0.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#include <ph4502c_sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "SensorParameters.h"

RF24 nrf24(9, 10);  //CE CSN
byte address[6] = "C0F33";

struct payload {
  uint8_t data_id;
  float paraData = 0.0;
};

payload data;

#define id_SurfaceTemperature 0x01
#define id_m5Humidity 0x02
#define id_m5Temperature 0x03
#define id_aht20Humidity 0x04
#define id_aht20Temperature 0x05
#define id_tdsValue 0x06
#define id_pH4052C 0x07

void setup() {
  Serial.begin(115200);
  Serial.println("\nAnaerobic Coffee Fermentation Vessel \n\n\nSYSTEM STARTING...");

  Wire.begin();

  // initialize sensor with normal i2c-address
  m5.init();

  if (!aht20.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);       //reference voltage on ADC, default 5.0V on  Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();            //initialization

  // Initialize the PH4502 instance
  ph4502.init();

  dallas_sensors.begin();  // Start up the library

  nrf24.begin();
  nrf24.setPALevel(RF24_PA_MAX);
  //nrf24.setChannel(115);
  nrf24.setDataRate(RF24_250KBPS);
  nrf24.openWritingPipe(address);
}


void loop() {
  // put your main code here, to run repeatedly:
  // read from first sensor
  Serial.println("\n\n\n############################################");

  //printData_SurfaceTemp();

  //printData_m5();

  aht20.getEvent(&aht20_humidity, &aht20_temp);  // populate temp and humidity objects with fresh data
  //printData_AHT20();

  //printData_TDS();

  //printData_pH4052C();

  nrf_mngr();

  Serial.print("############################################");



  delay(1000);
}


void nrf_mngr() {

  Serial.println("NRF Send Payload: ");
  Serial.print("[Surface Temperature= ");
  Serial.print(dallas_sensors.getTempCByIndex(0));  //Degree Celcius
  Serial.println("]");
  nrf_encrypt(id_SurfaceTemperature, dallas_sensors.getTempCByIndex(0));
  delay(6);

  Serial.print("[m5 Humidity= ");
  Serial.print(m5.getHumidity());  //%
  Serial.println("]");
  nrf_encrypt(id_m5Humidity, m5.getHumidity());
  delay(6);

  Serial.print("[m5 Temperature= ");
  Serial.print(m5.getTemperature());  //Degree Celcius
  Serial.println("]");
  nrf_encrypt(id_m5Temperature, m5.getTemperature());
  delay(6);

  Serial.print("[aht20 Humidity= ");
  Serial.print(aht20_humidity.relative_humidity);  //%
  Serial.println("]");
  nrf_encrypt(id_aht20Humidity, aht20_humidity.relative_humidity);
  delay(6);

  Serial.print("[aht20 Temperature= ");
  Serial.print(aht20_temp.temperature);  //Degree Celcius
  Serial.println("]");
  nrf_encrypt(id_aht20Temperature, aht20_temp.temperature);
  delay(6);

  gravityTds.setTemperature(dallas_sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
  gravityTds.update();                                           //sample and calculate
  Serial.print("[TDS Value= ");
  Serial.print(gravityTds.getTdsValue());  //ppm
  Serial.println("]");
  nrf_encrypt(id_tdsValue, gravityTds.getTdsValue());
  delay(6);

  Serial.print("[pH= ");
  Serial.print(ph4502.read_ph_level());  //pH
  Serial.println("]");
  nrf_encrypt(id_pH4052C, ph4502.read_ph_level());
  delay(6);
}


void nrf_encrypt(uint8_t id, float dataVal) {
  data.data_id = id;
  data.paraData = dataVal;
  nrf24.write(&data, sizeof(payload));
  Serial.println("Package:");
  if (data.data_id <= 0x0F) {
    Serial.print("0x0");
    Serial.println(data.data_id, HEX);
  } else {
    Serial.print("0x");
    Serial.println(data.data_id, HEX);
  }
  Serial.println(data.paraData);
}