#include <Wire.h>
#include "SHTSensor.h"

SHTSensor sht1(SHTSensor::SHT3X);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(1000); // let serial console settle



  // initialize sensor with normal i2c-address
  sht1.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  // read from first sensor
  if (sht1.readSample()) {
    Serial.print("SHT1 :\n");
    Serial.print("  RH: ");
    Serial.print(sht1.getHumidity(), 2);
    Serial.print("\n");
    Serial.print("  T:  ");
    Serial.print(sht1.getTemperature(), 2);
    Serial.print("\n");
  } else {
    Serial.print("Sensor 1: Error in readSample()\n");
  }


  delay(1000);
}