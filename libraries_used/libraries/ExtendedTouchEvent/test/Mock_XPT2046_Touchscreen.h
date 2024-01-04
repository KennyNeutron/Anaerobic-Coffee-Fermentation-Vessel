#ifndef MOCK_XPT2046_TOUCHSCREEN_H_
#define MOCK_XPT2046_TOUCHSCREEN_H_

#include <XPT2046_Touchscreen.h>
#include <gmock/gmock.h> 


class Mock_XPT2046_Touchscreen : public XPT2046_Touchscreen {
public:
    Mock_XPT2046_Touchscreen();
    ~Mock_XPT2046_Touchscreen();


	MOCK_METHOD(void, setRotation, (uint8_t n));
};

#endif