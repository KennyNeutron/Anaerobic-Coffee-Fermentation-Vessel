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
#include "CO2Sensor.h"
#include "DFRobot_OxygenSensor.h"
#include "SensorParameters.h"
/**
 * i2c slave Address, The default is ADDRESS_3.
 * ADDRESS_0   0x70  i2c device address.
 * ADDRESS_1   0x71
 * ADDRESS_2   0x72
 * ADDRESS_3   0x73
 */
#define Oxygen_IICAddress ADDRESS_3
#define COLLECT_NUMBER 10  // collect number, the collection range is 1-100.
DFRobot_OxygenSensor oxygen;

CO2Sensor co2Sensor(A6, 0.99, 100);

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
#define id_CO2 0x08
#define id_O2 0x09

#define LED_green 2
#define LED_red 3

#define ThisDevice '6'
void setup() {
  address[4] =  ThisDevice;
  pinMode(LED_green, OUTPUT);
  pinMode(LED_red, OUTPUT);

  for (int i = 0; i <= 10; i++) {
    digitalWrite(LED_green, 1); 
    digitalWrite(LED_red, 0);
    delay(50);
    digitalWrite(LED_green, 0);
    digitalWrite(LED_red, 1);
    delay(50);
  }
  digitalWrite(LED_green, 0);
  digitalWrite(LED_red, 0);

  Serial.begin(115200);
  Serial.println("\nAnaerobic Coffee Fermentation Vessel \nSYSTEM STARTING...\n");

  if (nrf24.begin()) {
    Serial.println(F("NRF24L01 HARDWARE DETECTED SUCCESSFULLY!"));
  } else {
    Serial.println(F("NRF24L01 HARDWARE NOT FOUND!"));
  }
  nrf24.setPALevel(RF24_PA_MAX);
  //nrf24.setChannel(115);
  nrf24.setDataRate(RF24_250KBPS);
  nrf24.openWritingPipe(address);
  nrf24.stopListening();

  Wire.begin();

  // initialize sensor with normal i2c-address
  m5.init();

  if (!aht20.begin()) {
    Serial.println("Could not find AHT? Check wiring");
  }
  Serial.println("AHT10 or AHT20 found");

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);       //reference voltage on ADC, default 5.0V on  Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();            //initialization

  // Initialize the PH4502 instance
  ph4502.init();

  dallas_sensors.begin();  // Start up the library

  Serial.println("=== CO2 Initializing ===");
  co2Sensor.calibrate();

  Serial.println("=== O2 Initializing ===");
  while (!oxygen.begin(Oxygen_IICAddress)) {
    Serial.println("I2c device number error !");
    delay(1000);
  }
  Serial.println("Device connected successfully !");
}


void loop() {
  // put your main code here, to run repeatedly:
  // read from first sensor
  Serial.println("\n\n\n############################################");

  printData_SurfaceTemp();

  //printData_m5();

  aht20.getEvent(&aht20_humidity, &aht20_temp);  // populate temp and humidity objects with fresh data
  //printData_AHT20();

  //printData_TDS();

  //printData_pH4052C();

  nrf_mngr();

  Serial.print("############################################");

  //delay(1000);
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

  int co2_val = co2Sensor.read();
  Serial.print("[CO2=");
  Serial.print(co2_val);
  Serial.println("]");
  nrf_encrypt(id_CO2, co2_val);
  delay(6);

  float oxygenData = oxygen.getOxygenData(COLLECT_NUMBER);
  Serial.print("[O2=");
  Serial.print(oxygenData);
  Serial.println("]");
  nrf_encrypt(id_O2, oxygenData);
  delay(6);
}


void nrf_encrypt(uint8_t id, float dataVal) {

  digitalWrite(LED_green, 1);
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

  Serial.println("NRF SENT DATA");

  digitalWrite(LED_green, 0);
}