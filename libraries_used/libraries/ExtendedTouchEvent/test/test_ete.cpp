
#include <ExtendedTouchEvent.h>
#include <gtest/gtest.h>

#include "Mock_Adafruit_GFX.h"
#include "Mock_XPT2046_Touchscreen.h"

Mock_XPT2046_Touchscreen ts;
Mock_Adafruit_GFX gfx;

ExtendedTouchEvent ete = ExtendedTouchEvent(ts, gfx);

using ::testing::Eq;
using ::testing::Matcher;

TEST(test_ExtendedTouchEvent, test_setRotation) {
    EXPECT_CALL(gfx, setRotation(Matcher<uint8_t>(Eq(2)))).Times(1);
    EXPECT_CALL(ts, setRotation(Matcher<uint8_t>(Eq(2)))).Times(1);

    ete.setRotation(2);
}

TEST(test_ExtendedTouchEvent, test_isInRectangle) {
    TS_Point p = TS_Point(20, 30, 8);

    EXPECT_TRUE(ete.isInRectangle(p, 2, 5, 20, 30));
    EXPECT_TRUE(ete.isInRectangle(p, 2, 5, 18, 25));
    EXPECT_FALSE(ete.isInRectangle(p, 2, 5, 17, 25));
    EXPECT_FALSE(ete.isInRectangle(p, 2, 5, 18, 24));
}

TEST(test_ExtendedTouchEvent, test_isInCircle) {
    EXPECT_TRUE(ete.isInCircle(TS_Point(20, 28, 8), 2, 5, 30));
    EXPECT_FALSE(ete.isInCircle(TS_Point(20, 30, 8), 2, 5, 30));
}

// 
#define X_OFFSET 9
#define Y_OFFSET 4

TEST(test_ExtendedTouchEvent, test_isInPolygon) {
    int16_t vert[][2] = {{10, 10}, {15, 15}, {15, 10}, {20, 12}, {12, 5}};
    char *data[] = {
      "             ", // effective Y = 4 (= Y_OFFSET)
      "   .         ",
      "   ..        ", 
      "   ...       ",
      "  .....      ", 
      "  ......     ",
      " ........    ", 
      "  ....   .   ",
      "   ...     . ", 
      "    ..       ",
      "     .       ", 
      "      .      ",
      "             "
    };

    for (int y = 0; y < 13; y++) {
        char *row = data[y];
        for (int x = 0; x < 12; x++) {
            char mes[20];
            snprintf(mes, 20, "x = %d, y = %d", x, y);

            EXPECT_EQ(row[x] == '.', ete.isInPolygon(TS_Point(x + X_OFFSET, y + Y_OFFSET, 8), 5, vert)) << mes;
        }
    }

    EXPECT_FALSE(ete.isInPolygon(TS_Point(20, 30, 8), 5, vert));
}

#if defined(ESP32) || defined(ESP8266)
#ifdef ESP32
#define SPEED 921600
#else
#define SPEED 460800
#endif
#endif

void setup() {
    Serial.begin(SPEED);
    delay(1000);
    testing::InitGoogleTest();
    if (RUN_ALL_TESTS()) delay(50);

}

void loop() {
    // nothing to be done here.
    delay(100);
}