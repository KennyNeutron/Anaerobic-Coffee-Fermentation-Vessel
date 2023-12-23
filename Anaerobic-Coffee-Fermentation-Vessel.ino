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

#define TdsSensorPin A0

// Define the pH level pin and temperature pin
#define PH4502C_PH_LEVEL_PIN A1
#define PH4502C_TEMP_PIN A2

#define ONE_WIRE_BUS 5


// Create an instance of the PH4502C_Sensor
PH4502C_Sensor ph4502(PH4502C_PH_LEVEL_PIN, PH4502C_TEMP_PIN);
GravityTDS gravityTds;

float tdsValue = 0;

SHTSensor m5(SHTSensor::SHT3X);

Adafruit_AHTX0 aht20;

OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature dallas_sensors(&oneWire);

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
}


void loop() {
  // put your main code here, to run repeatedly:
  // read from first sensor
  Serial.print("\n\n\n############################################\n\n\n");

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

  sensors_event_t aht20_humidity, aht20_temp;
  aht20.getEvent(&aht20_humidity, &aht20_temp);  // populate temp and humidity objects with fresh data
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

  Serial.print("\n\n\n");
  Serial.println("================================================");
  Serial.println("TDS Sensor:");
  gravityTds.setTemperature(dallas_sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
  gravityTds.update();                                //sample and calculate
  tdsValue = gravityTds.getTdsValue();                // then get the value
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  Serial.println("================================================");

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



  Serial.print("\n\n\n############################################");

  delay(1000);
}