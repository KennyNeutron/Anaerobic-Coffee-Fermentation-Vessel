/* This example demonstrates how to use the ExtendedTouchEvent library, especially usage of the
 * different methods for registering callbacks for eventhandling.
 *
 * It uses a 2.4" TFT display (with  a ILI9341 controller) with resistive touch screen (XPT2046).
 * The module is used together with an ESP32 but any other arduino compatible processor should work
 * For the used pins see definitions in the code
 *
 */

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <Fonts/FreeSans9pt7b.h>

#include "ExtendedTouchEvent.h"

//used pins
#define TFT_CS   5      //diplay chip select
#define TFT_DC   4      //display d/c
#define TFT_MOSI 23     //diplay MOSI
#define TFT_CLK  18     //display clock
#define TFT_RST  22     //display reset
#define TFT_MISO 19     //display MISO
#define TFT_LED  15     //display background LED

#define TOUCH_CS 14     //touch screen chip select
#define TOUCH_IRQ 2     //touch screen interrupt


//Messbereich muss eventuell kalibriert werden
#define TS_SMALLMIN 250
#define TS_LONGMIN 250
#define TS_SMALLMAX 3850
#define TS_LONGMAX 3900


//prepare driver for display and touch screen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

//init ExtendedTouchEvent with pointer to the touch screen driver
ExtendedTouchEvent tevent(touch, tft);

// some global variables
int scr = 0;
uint16_t bg[4] = { ILI9341_WHITE, ILI9341_RED, ILI9341_GREEN, ILI9341_BLUE };
TS_Point last;
boolean draw;

//fill screen with different colors for different screen numbers
void draw_screen(uint8_t nr) {
    tft.fillScreen(bg[nr]);
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setCursor(10, 20);
    tft.print("Screen Nr. ");
    tft.print(nr);
}

// wipe event
void onWipe(WIPE_DIRECTION dir) {
    switch (dir) {
        case WIPE_DIRECTION::WD_LEFT_RIGHT:
            if (scr < 3) { //right to left switch to next screen
                scr++;
                draw_screen(scr);
            }
            break;
        case WIPE_DIRECTION::WD_RIGHT_LEFT:
            if (scr > 0) { //left to right switch to previous screen
                scr--;
                draw_screen(scr);
            }
            break;
    }
    tevent.setDrawMode(scr == 0);
}

void onClick(TS_Point p) {
    if (scr > 0) { //on any screen except 0 show the click position
        tft.setFont(& FreeSans9pt7b);
        tft.fillRect(0, 40, 240, 70, ILI9341_WHITE);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setCursor(10, 60);
        tft.println("Clicked");
        tft.println(p.x);
        tft.println(p.y);
    }
}

void onDblClick(TS_Point p) {
    if (scr > 0) { //on any screen except 0 show double click position
        tft.fillRect(0, 40, 240, 70, ILI9341_WHITE);
        tft.setFont(& FreeSans9pt7b);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setCursor(10, 60);
        tft.println("Doubleclick");
        tft.println(p.x);
        tft.println(p.y);
    } else {
        tevent.setDrawMode(false);
    }
}

void onDraw(TS_Point p) {
    //draw a line from the last position to the current position
    if (draw) {
        tft.drawLine(last.x, last.y, p.x, p.y, ILI9341_BLACK);
    }
    last = p;
}

void onTouch(TS_Point p) {
    last = p;
    draw = true;
}

void onUntouch(TS_Point p) {
    draw = false;
}

void onLongClick(TS_Point p) {
    if (scr > 0) { //on any screen except 0 show double click position
        tft.setFont(&FreeSans9pt7b);
        tft.fillRect(0, 40, 240, 70, ILI9341_WHITE);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setCursor(10, 60);
        tft.println("Long Click");
        tft.println(p.x);
        tft.println(p.y);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(TFT_LED, OUTPUT);
    // other displays may need HIGH to be lit
    digitalWrite(TFT_LED, LOW);    // switch display on
    //start drivers
    tft.begin();
    touch.begin();
    //show the displays resolution
    Serial.printf("w =%d,  h=%d\n", tft.width(), tft.height());
    //init ExtendedTouchEvent instance
    tevent.setResolution(tft.width(), tft.height());
    tevent.calibrate(TS_SMALLMIN, TS_LONGMIN, TS_SMALLMAX, TS_LONGMAX);
    tevent.setDblClick(300);
    tevent.setRotation(0);
    tevent.registerOnTouchWipe(onWipe);
    tevent.registerOnTouchClick(onClick);
    tevent.registerOnTouchDblClick(onDblClick);
    tevent.registerOnTouchLong(onLongClick);
    tevent.registerOnTouchDraw(onDraw);
    tevent.registerOnTouchDown(onTouch);
    tevent.registerOnTouchUp(onUntouch);
    //display
    draw_screen(scr);
}

void loop() {
    //poll for touch events
    tevent.pollTouchScreen();
}
