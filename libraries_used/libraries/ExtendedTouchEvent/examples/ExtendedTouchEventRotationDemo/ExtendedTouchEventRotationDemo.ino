/* This example demonstrates how to use the ExtendedTouchEvent library, especially usage of the methods
 * setRotation(), isInRectangle() and lastTouchEventTime(). It also implements a simple screensaver which dims screen
 * and sets processor into light sleep in case of longer inactivity.
 *
 * if USE_PWM is defined, brightness of display will be controlled by y-coordinate of last click (the higher the brighter)
 * using PWM. Otherwise the screensaver simply switches on and off.
 *
 * It uses a 2.4" TFT display (with  a ILI9341 controller) with resistive touch screen (XPT2046).
 * The module is used together with an ESP32 but any other arduino compatible processor should work
 * For the used pins see definitions in the code
 *
 */
#include "Arduino.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "XPT2046_Touchscreen.h"
#include "Fonts/FreeSans9pt7b.h"
#include "ExtendedTouchEvent.h"
#include "esp_sleep.h"

/* if USE_PWM is defined, brightness of display will be controlled by y-coordinate of last click (the higher the brighter)
 * using PWM otherwise screensaver switches only on and off */
#define USE_PWM

//used pins
#define TFT_CS   5      //diplay chip select
#define TFT_DC   4      //display d/c
#define TFT_MOSI 23     //diplay MOSI
#define TFT_CLK  18     //display clock
#define TFT_RST  22     //display reset
#define TFT_MISO 19     //display MISO
#define TFT_LED  15     //display background LED

#define HAVE_TOUCHPAD
#define TOUCH_CS 14     //touch screen chip select
#define TOUCH_IRQ 2     //touch screen interrupt

// Parameter für Touchscreen
#define ILI9341_ULTRA_DARKGREY    0x632C
#define MINPRESSURE 10
#define MAXPRESSURE 2000
//following values might to be altered according to the hardware
#define TS_SMALLMIN 250
#define TS_LONGMIN 250
#define TS_SMALLMAX 3850
#define TS_LONGMAX 3900

//Instances of touchscreen and TFT
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

//init TouchEvent with pointer to the touch screen driver
ExtendedTouchEvent tevent(touch, tft, 200);

// coordinates of last touch
int tsx, tsy;
// current touch state
bool tsdown = false;
// current screen orientation 0 = portrait, ..., 3 = landscape reverse
uint8_t rotation = 0;

// time of last touch (values see millis())
unsigned long lastknowntouch = 0;
// desired state of backlights
boolean light_on = true;

void drawCycle();

void handleTouchDown(TS_Point p) {
	tsdown = true;
	drawCycle();
}
//#####################################################

void handleTouchUp(TS_Point p) {
	tsdown = false;
	drawCycle();
}
//#####################################################

void handleTouchEvent(TS_Point p) {
	tsx = p.x; //x und y als Rohwerte merken
	tsy = p.y;
	delay(1);

	// get screen orientation
	uint8_t rot = tft.getRotation();

	// check if touch is inside of green square

	int w = tft.width();
	int h = tft.height();

	boolean b;
	if ((rot == 1) || (rot == 3)) { // landscape
		b = tevent.isInRectangle(p, w / 2 - 10, h / 2 - 20, 20, 40);
	} else { // portrait
		b = tevent.isInRectangle(p, w / 2 - 20, h / 2 - 10, 40, 20);
	}

	if (b) {
		Serial.printf("in green square w=%d, h=%d\n", tft.width(), tft.height());
		// change orientation
		rotation++;
		if (rotation > 3)
			rotation = 0;

		Serial.printf("rotation=%d\n", rotation);
		tevent.setRotation(rotation);
	}

	// redraw screen
	draw_screen();
}
//#####################################################

// switch off display
void dim() {
    light_on = false;
    Serial.println("dim");
#ifdef USE_PWM
    ledcWrite(0, 256);
#else
    // other displays might use LOW for off
    digitalWrite(TFT_LED, HIGH);
#endif
}
//#####################################################

// switch on display
void enlighten() {
    Serial.println("enlighten");
    light_on = true;
#ifdef USE_PWM
    ledcWrite(0, min(tsy, 255));
#else
    // other displays might use HIGH for on
    digitalWrite(TFT_LED, LOW);
#endif
}
//#####################################################

// time in ms, since last touch
unsigned long notTouchedSince() {
	unsigned long now = millis();
	if (lastknowntouch <= now) {
		return now - lastknowntouch;
	} else {
	    // millis has an overflow every 50 days ;-)
		return ULONG_MAX - lastknowntouch + now;
	}
}
//#####################################################

// switch off backgroundlights in case of longer inactivity, switch on 1st touch
void screensaver() {
	unsigned long lasttouch = tevent.lastTouchEventTime();

	if (lastknowntouch != lasttouch) {
		lastknowntouch = lasttouch;
#ifdef USE_PWM
        enlighten();
#else
        if (! light_on) {
            enlighten();
        }
#endif
	} else {
		if (notTouchedSince() > 20000) { // TODO time configurable
			if (light_on) {
				dim();
			}
		}
	}
}
//#####################################################

// drwo the yellow / red circle
void drawCycle() {
	if ((rotation == 1) || (rotation == 3)) {
		if (tsdown)
			tft.fillCircle(160, 160, 10, ILI9341_RED);
		else
			tft.fillCircle(160, 160, 10, ILI9341_YELLOW);
	} else {
		if (tsdown)
			tft.fillCircle(120, 200, 10, ILI9341_RED);
		else
			tft.fillCircle(120, 200, 10, ILI9341_YELLOW);
	}
}
//#####################################################

// display a number
void display_right(int x, int y, String val) {
	int16_t x1, y1;
	uint16_t w, h;
	int str_len = val.length() + 1;
	char char_array[str_len];
	val.toCharArray(char_array, str_len);
	tft.getTextBounds(char_array, x, y, &x1, &y1, &w, &h);
	tft.setCursor(x - w, y);
	tft.print(char_array);
}
//#####################################################

// show position in landscape orientation
void drawPositionLandscape(uint16_t x, uint16_t y) {
	tft.setCursor(20, 60);
	tft.print("X = ");
	display_right(110, 60, String(x));
	tft.setCursor(180, 60);
	tft.print("Y = ");
	display_right(270, 60, String(y));

	drawCycle();
}
//#####################################################

// show position in portrait orientation
void drawPositionPortrait(uint16_t x, uint16_t y) {
	tft.setCursor(20, 60);
	tft.print("X = ");
	display_right(110, 60, String(x));
	tft.setCursor(20, 100);
	tft.print("Y = ");
	display_right(110, 100, String(y));

	drawCycle();
}
//#####################################################

// draw the complete screen
void draw_screen() {
	uint16_t w, h;
	// choose background colour and font
	tft.setRotation(rotation);
	tft.fillScreen(0x001A); // Dunkleres Blau
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setFont(&FreeSans9pt7b);
	w = tft.width();
	h = tft.height();
	// draw messured positions and  green square
	if ((rotation == 1) || (rotation == 3)) {
		drawPositionLandscape(tsx, tsy);
		tft.fillRect(w / 2 - 10, h / 2 - 20, 20, 40, ILI9341_GREEN);
	} else {
		drawPositionPortrait(tsx, tsy);
		tft.fillRect(w / 2 - 20, h / 2 - 10, 40, 20, ILI9341_GREEN);
	}

	// show rotations index in green square
	tft.setCursor(w / 2 - 5, h / 2 + 6);
	tft.setTextColor(ILI9341_BLACK, ILI9341_GREEN);
	tft.print(rotation);

	// draw arrows to the corners
	tft.drawLine(0, 0, 20, 0, ILI9341_WHITE);
	tft.drawLine(0, 0, 0, 20, ILI9341_WHITE);
	tft.drawLine(0, 0, 40, 40, ILI9341_WHITE);

	tft.drawLine(w - 1, 0, w - 20, 0, ILI9341_YELLOW);
	tft.drawLine(w - 1, 0, w - 1, 20, ILI9341_YELLOW);
	tft.drawLine(w - 1, 0, w - 40, 40, ILI9341_YELLOW);

	tft.drawLine(w - 1, h - 1, w - 40, h, ILI9341_CYAN);
	tft.drawLine(w - 1, h - 1, w, h - 40, ILI9341_CYAN);
	tft.drawLine(w - 1, h - 1, w - 40, h - 40, ILI9341_CYAN);

	tft.drawLine(0, h - 1, 20, h - 1, ILI9341_CASET);
	tft.drawLine(0, h - 1, 0, h - 20, ILI9341_CASET);
	tft.drawLine(0, h - 1, 40, h - 40, ILI9341_CASET);
}
//#####################################################

// start light sleep if necessary
void sleep() {
	if (notTouchedSince() > 30000) { // TODO configurable
	    Serial.println("go to sleep");
		delay(100); // wait a bit for completion of output

		esp_err_t ws = gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_LOW_LEVEL);
		if (ws != ESP_OK) {
		    Serial.printf("gpio_wakeup_enable failed %d", ws);
			return;
		}

		ws = esp_sleep_enable_gpio_wakeup();
		if (ws != ESP_OK) {
		    Serial.printf("esp_sleep_enable_gpio_wakeup failed %d", ws);
			return;
		}

		ws = esp_sleep_enable_timer_wakeup(30000000L); // 30 sec schlafen
		if (ws != ESP_OK) {
		    Serial.printf("esp_sleep_enable_timer_wakeup failed %d", ws);
			return;
		}

		Serial.println("good night\n");
		delay(100);
		ws = esp_light_sleep_start();
		if (ws != ESP_OK) {
		    Serial.printf("esp_light_sleep_start failed %d", ws);
		} else {
			esp_sleep_wakeup_cause_t wach = esp_sleep_get_wakeup_cause();
			if (wach == ESP_SLEEP_WAKEUP_GPIO) {
			    Serial.println("touch wake");
				enlighten();
			} else if (wach == ESP_SLEEP_WAKEUP_TIMER) {
			    Serial.println("timer wake");
			}
		}
	}
}
//#####################################################


void setup() {
    Serial.begin(115200);
	pinMode(TFT_LED, OUTPUT);

#ifdef USE_PWM
	ledcSetup(0, 5000, 8);
	ledcAttachPin(TFT_LED, 0);
#endif

	enlighten ();
	// start drivers
	tft.begin();
	touch.begin();

	// resolution of display
	Serial.printf("tft.w=%d,  tft.h =%d\n", tft.width(), tft.height());

	//init ExtendedTouchEvent instance
	tevent.setResolution(tft.width(), tft.height());
	tevent.setDblClick(300);
	tevent.registerOnTouchClick(handleTouchEvent);
	tevent.registerOnTouchDown(handleTouchDown);
	tevent.registerOnTouchUp(handleTouchUp);

	tevent.calibrate(TS_SMALLMIN, TS_LONGMIN, TS_SMALLMAX, TS_LONGMAX);

	tsx = 0;
	tsy = 0;
	tsdown = false;
	rotation = 0;
	// display screen
	tevent.setRotation(rotation);
	draw_screen();
}
//#####################################################

void loop() {
	//poll for touch events
	tevent.pollTouchScreen();
	screensaver();
	sleep();
}
