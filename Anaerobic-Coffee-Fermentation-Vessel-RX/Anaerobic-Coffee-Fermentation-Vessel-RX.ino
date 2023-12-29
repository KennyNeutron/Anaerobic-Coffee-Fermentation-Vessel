/*
    Anaerobic Coffee Fermentation Vessel
    Reciever Code
    Version 1.0.0

    by: Kenny Neutron

    Environment: Arduino Mega 2560

    TFT DISPLAY: MAR4018
    SCREEN SIZE: 4.0"
    RESOLUTION: 800*480 Pixels
    DRIVER: MAR4018
    TOUCH IC: XPT2046

    Date Started: 12-20-2023
    Date Finished: N/A
*/
#include "Arduino.h"
#include <SPI.h>
#include <SD.h>           // SD Card Library

#include <nRF24L01.h>
#include <RF24.h>
#include "DigitalIO.h"


#include <LCDWIKI_GUI.h>  //Core graphics library
#include <LCDWIKI_KBV.h>  //Hardware-specific library
#include <XPT2046_Touchscreen.h>

#define TCS_PIN 53

XPT2046_Touchscreen ts(TCS_PIN);
//#define TIRQ_PIN 44
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(TCS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
//the definiens of 8bit mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
LCDWIKI_KBV tft(NT35510, 40, 38, 39, 43, 41);  //model,cs,cd,wr,rd,reset

/*  r     g    b */
#define BLACK 0x0000       /*   0,   0,   0 */
#define BLUE 0x001F        /*   0,   0, 255 */
#define RED 0xF800         /* 255,   0,   0 */
#define GREEN 0x07E0       /*   0, 255,   0 */
#define CYAN 0x07FF        /*   0, 255, 255 */
#define MAGENTA 0xF81F     /* 255,   0, 255 */
#define YELLOW 0xFFE0      /* 255, 255,   0 */
#define WHITE 0xFFFF       /* 255, 255, 255 */
#define NAVY 0x000F        /*   0,   0, 128 */
#define DARKGREEN 0x03E0   /*   0, 128,   0 */
#define DARKCYAN 0x03EF    /*   0, 128, 128 */
#define MAROON 0x7800      /* 128,   0,   0 */
#define PURPLE 0x780F      /* 128,   0, 128 */
#define OLIVE 0x7BE0       /* 128, 128,   0 */
#define LIGHTGREY 0xC618   /* 192, 192, 192 */
#define DARKGREY 0x7BEF    /* 128, 128, 128 */
#define ORANGE 0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5 /* 173, 255,  47 */
#define PINK 0xF81F        /* 255,   0, 255 */

RF24 nrf(3, 4);  //CE CSN
byte address[6] = "C0F33";

/*
  used SOFTWARE SPI on the NRF24L01
  CE    --> pin3
  CSN   --> pin4
  SCK   --> pin5
  MOSI  --> pin6
  MISO  --> pin7
*/

struct payload {
  uint8_t data_id;
  float paraData = 0.0;
};

payload data;

void setup() {
  Serial.begin(115200);
  Serial.println("\nAnaerobic Coffee Fermentation Vessel Receiver\n\n\nSYSTEM STARTING...");
  delay(1000);


  nrf.begin();
  //nrf.setChannel(115);
  nrf.setPALevel(RF24_PA_MAX);
  nrf.setDataRate(RF24_250KBPS);
  nrf.openReadingPipe(0, address);
  nrf.startListening();

  tft.Init_LCD();
  tft.Set_Rotation(1);

  ts.begin();
  ts.setRotation(3);


  //Init SD_Card
  pinMode(48, OUTPUT);
  
  if (!SD.begin(48)) {
    tft.Set_Text_Back_colour(BLUE);
    tft.Set_Text_colour(WHITE);
    tft.Set_Text_Size(1);
    tft.Print_String("SD Card Init fail!", 0, 0);
  }

  tft.Fill_Screen(RED);
  tft.Fill_Screen(GREEN);
  tft.Fill_Screen(BLUE);

  //disp_LoadingScreen();

  delay(2000);
  tft.Fill_Screen(BLACK);
}

void loop() {

  if (nrf.available()) {
    while (nrf.available()) {
      nrf.read(&data, sizeof(data));
    }
    Serial.println("\nPackage:");
    Serial.println(data.data_id);
    Serial.println(data.paraData);
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("\nPackage:");
    dataFile.println(data.data_id);
    dataFile.println(data.paraData);
    dataFile.close();
    // print to the serial port too:
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}