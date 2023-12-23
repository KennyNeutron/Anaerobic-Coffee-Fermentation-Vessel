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

SHTSensor m5(SHTSensor::SHT3X);



void setup(){
    Serial.begin(115200);
    Serial.println("\nAnaerobic Coffee Fermentation Vessel \n\n\nSYSTEM STARTING...");

  Wire.begin();

  // initialize sensor with normal i2c-address
  m5.init();
}


void loop(){
  // put your main code here, to run repeatedly:
  // read from first sensor
  if (m5.readSample()) {
    Serial.print("SHT1 :\n");
    Serial.print("  RH: ");
    Serial.print(m5.getHumidity(), 2);
    Serial.print("\n");
    Serial.print("  T:  ");
    Serial.print(m5.getTemperature(), 2);
    Serial.print("\n");
  } else {
    Serial.print("Sensor 1: Error in readSample()\n");
  }


  delay(1000);
}