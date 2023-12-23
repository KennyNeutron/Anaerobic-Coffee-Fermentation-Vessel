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

SHTSensor m5(SHTSensor::SHT3X);

Adafruit_AHTX0 aht20;

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
}


void loop() {
  // put your main code here, to run repeatedly:
  // read from first sensor
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
  Serial.println("\n\n");
  Serial.println("================================================");
  Serial.println("AHT20 Sensor:");
  Serial.print("Temperature: ");
  Serial.print(aht20_temp.temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(aht20_humidity.relative_humidity);
  Serial.println("% rH");
  Serial.println("================================================");

  delay(1000);
}