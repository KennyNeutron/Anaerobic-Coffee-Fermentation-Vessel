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
#include "Variables.h"
#include <SPI.h>
#include <SD.h>  // SD Card Library

#include <nRF24L01.h>
#include <RF24.h>
#include "DigitalIO.h"


#include <LCDWIKI_GUI.h>  //Core graphics library
#include <LCDWIKI_KBV.h>  //Hardware-specific library
#include <XPT2046_Touchscreen.h>

#include <DS3231.h>
#include <Wire.h>


DS3231 myRTC;

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

byte year;
byte month;
byte date;
byte dow;
byte hour;
byte minute;
byte second;

bool century = false;
bool h12Flag;
bool pmFlag;

uint32_t display_last_millis = 0;

bool transmitterAvailable = false;
bool transmitterAvailableToggle = false;

bool loggingToggle = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\nAnaerobic Coffee Fermentation Vessel Receiver\n\n\nSYSTEM STARTING...");
  delay(1000);

  Wire.begin();


  //SET DATE & TIME
  //DS3231_setTime(13, 53, 0, 12, 29, 23);  //Hour-Minute-Second-Month-Date-Year (24HR FORMAT)


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
  /*
  tft.Fill_Screen(RED);
  tft.Fill_Screen(GREEN);
  tft.Fill_Screen(BLUE);

  disp_LoadingScreen();

  Serial.println("DS3231 Time: " + DS3231_getTimeString());
  Serial.println("DS3231 Date: " + DS3231_getDateString());

  delay(3000);
  */
  tft.Fill_Screen(BLACK);
  display_last_millis = millis();
}

void loop() {

  if (nrf.available()) {
    while (nrf.available()) {
      nrf.read(&data, sizeof(data));
    }
    nrf24L01_decrypt(data.data_id, data.paraData);
    transmitterAvailable = true;

    //Serial.println("\nPackage:");
    //Serial.println(data.data_id);
    //Serial.println(data.paraData);
  } else {
    if (transmitterAvailableToggle) {
      transmitterAvailable = false;
      transmitterAvailableToggle = false;
    }
  }

  if (millis() - display_last_millis > 600) {
    show_string("TIME: " + DS3231_getTimeString(), 0, 0, 3, WHITE, BLACK, 0);
    show_string("DATE:" + DS3231_getDateString(), 0, 30, 3, WHITE, BLACK, 0);

    show_string("SURFACE TEMPERATURE: ", 0, 100, 3, WHITE, BLACK, 0);
    show_string(String(SurfaceTemperature ) + "degC", 360, 100, 3, RED, BLACK, 0);

    show_string("M5 Humidity: ", 0, 140, 3, WHITE, BLACK, 0);
    show_string(String(m5Humidity) + "%", 230, 140, 3, GREEN, BLACK, 0);

    show_string("M5 Temperature: ", 0, 180, 3, WHITE, BLACK, 0);
    show_string(String(m5Temperature) + "degC", 270, 180, 3, RED, BLACK, 0);

    show_string("AHT20 Humidity: ", 0, 220, 3, WHITE, BLACK, 0);
    show_string(String(aht20Humidity) + "%", 270, 220, 3, GREEN, BLACK, 0);

    show_string("AHT20 Temperature: ", 0, 260, 3, WHITE, BLACK, 0);
    show_string(String(aht20Temperature) + "degC", 320, 260, 3, RED, BLACK, 0);

    show_string("TDS Value: ", 0, 300, 3, WHITE, BLACK, 0);
    show_string(String(tdsValue) + "ppm", 200, 300, 3, CYAN, BLACK, 0);

    show_string("pH4052C (pH): ", 0, 340, 3, WHITE, BLACK, 0);
    show_string(String(pH4052C), 250, 340, 3, YELLOW, BLACK, 0);

    display_last_millis = millis();

    if (transmitterAvailable && !transmitterAvailableToggle) {
      show_string("Transmitter Detected", 550, 0, 2, GREEN, BLACK, 0);
      transmitterAvailableToggle = true;
    } else {
      tft.Set_Draw_color(BLACK);
      tft.Fill_Rectangle(550, 0, 800, 15);
    }
  }


  if (myRTC.getSecond() == 0 && !loggingToggle) {
    Serial.println("\n\n\nDATA LOG!");
    Serial.println("DS3231 Time: " + DS3231_getTimeString());
    Serial.println("DS3231 Date: " + DS3231_getDateString());
    loggingToggle = true;

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println("\n\n\n================================================");
      dataFile.println("DS3231 Time: " + DS3231_getTimeString());
      dataFile.println("DS3231 Date: " + DS3231_getDateString());
      dataFile.println("Surface Temperature: " + String(SurfaceTemperature) + "degC");
      dataFile.println("M5 Humidity: " + String(m5Humidity) + "%");
      dataFile.println("M5 Temperature: " + String(m5Temperature) + "degC");
      dataFile.println("AHT20 Humidity: " + String(aht20Humidity) + "%");
      dataFile.println("AHT20 Temperature: " + String(aht20Temperature) + "degC");
      dataFile.println("TDS Value: " + String(tdsValue) + "ppm");
      dataFile.println("pH4052C (pH level): " + String(pH4052C));
      dataFile.println("================================================");
      dataFile.close();
      // print to the serial port too:
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  } else if (myRTC.getSecond() != 0) {
    loggingToggle = false;
  }
}