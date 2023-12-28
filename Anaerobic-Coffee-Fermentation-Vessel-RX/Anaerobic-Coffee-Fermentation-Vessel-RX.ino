#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 myRadio (6, 7); //CE CSN
byte address[6] = "C0F33";

struct payload {
  uint8_t data_id;
  float paraData = 0.0;
};

payload data;

void setup() {
  Serial.begin(115200);
  delay(1000);

  myRadio.begin();
  //myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(0, address);
  myRadio.startListening();
}

void loop() {
  if (myRadio.available()) {
    while (myRadio.available()) {
      myRadio.read(&data, sizeof(data));
    }
    Serial.println("\nPackage:");
    Serial.println(data.data_id);
    Serial.println(data.paraData);
  }
}