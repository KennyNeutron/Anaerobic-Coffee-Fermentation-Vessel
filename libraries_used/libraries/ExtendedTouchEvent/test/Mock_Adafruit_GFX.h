#ifndef MOCK_ADAFRUIT_GFX_H_
#define MOCK_ADAFRUIT_GFX_H_

#include <Adafruit_GFX.h>
#include <gmock/gmock.h> 

class Mock_Adafruit_GFX : public Adafruit_GFX {
public:
    Mock_Adafruit_GFX();


    MOCK_METHOD(void, drawPixel, (int16_t x, int16_t y, uint16_t color), (override));
    MOCK_METHOD(void, startWrite, (), (override));
    MOCK_METHOD(void, writePixel, (int16_t x, int16_t y, uint16_t color), (override));
    MOCK_METHOD(void, writeFastVLine, (int16_t x, int16_t y, int16_t h, uint16_t color), (override));
    MOCK_METHOD(void, writeFastHLine, (int16_t x, int16_t y, int16_t h, uint16_t color), (override));
    MOCK_METHOD(void, writeLine, (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color), (override));
    MOCK_METHOD(void, endWrite, (), (override));

    MOCK_METHOD(void, setRotation, (uint8_t r), (override));
    MOCK_METHOD(void, invertDisplay, (boolean i), (override));

    MOCK_METHOD(void, drawFastVLine, (int16_t x, int16_t y, int16_t h, uint16_t color), (override));
    MOCK_METHOD(void, drawFastHLine, (int16_t x, int16_t y, int16_t w, uint16_t color), (override));
    MOCK_METHOD(void, fillRect, (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color), (override));
    MOCK_METHOD(void, fillScreen, (uint16_t color), (override));
    MOCK_METHOD(void, drawLine, (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color), (override));
    MOCK_METHOD(void, drawRect, (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color), (override));

};

#endif