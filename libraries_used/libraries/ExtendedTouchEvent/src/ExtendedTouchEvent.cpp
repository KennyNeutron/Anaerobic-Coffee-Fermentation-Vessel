/*
 * ExtendedTouchEvent.cpp
 *
 *  Created on: 17.05.2020
 *      Author: arduhe
 */
#include "ExtendedTouchEvent.h"
#include <XPT2046_Touchscreen.h>
#include <Adafruit_GFX.h>

#define SQR(x) (x) * (x)

ExtendedTouchEvent::ExtendedTouchEvent(XPT2046_Touchscreen &touch, Adafruit_GFX &tft, int16_t black) :
        _touch(touch), _tft(tft), _black(black) {
}

ExtendedTouchEvent::ExtendedTouchEvent(XPT2046_Touchscreen &touch, Adafruit_GFX &tft) : _touch(touch), _tft(tft) {}

void ExtendedTouchEvent::setRotation(uint8_t n) {
    _touch.setRotation(n);
    _tft.setRotation(n);
}

boolean ExtendedTouchEvent::isInRectangle(TS_Point p, int16_t x, int16_t y, int16_t w, int16_t h) {
    return ((x <= p.x) && (p.x <= x + w) && (y <= p.y) && (p.y <= y + h));
}

unsigned long ExtendedTouchEvent::lastTouchEventTime() {
    return _lasttouch;
}
//#####################################################

boolean ExtendedTouchEvent::isInCircle(TS_Point p, int16_t x, int16_t y, int16_t r) {
    return SQR(x - p.x) + SQR(y - p.y) < SQR(r);
}
//#####################################################

boolean ExtendedTouchEvent::isInPolygon(TS_Point p, int16_t nvertices, int16_t vertices[][2]) {
    boolean c = false;
    for (int i = 0, j = nvertices - 1; i < nvertices; j = i++) {
        if ((p.x == vertices[i][0]) && (p.y == vertices[i][1])) {
            return true; // special case vertex
        }
        int dy = vertices[j][1] - vertices[i][1];
        if (((vertices[i][1] > p.y) != (vertices[j][1] > p.y)) &&
            ((dy < 0) ? (p.x * dy > ((vertices[j][0] - vertices[i][0]) * (p.y - vertices[i][1]) + vertices[i][0] * dy)) :
                        (p.x * dy < ((vertices[j][0] - vertices[i][0]) * (p.y - vertices[i][1]) + vertices[i][0] * dy)))) {
            c = ! c;
        }
    }
    return c;
}
//#####################################################

void ExtendedTouchEvent::pollTouchScreen() {
    TS_Point p, scr;
    boolean tch;
    int16_t dx, dy;
    //first we get position and check if touched
    p = _touch.getPoint();
    tch = (p.z > _minPress);

    uint32_t now = millis();

    if (tch && _down) { //continous touch
        scr = toScreen(p);

        dx = abs(_lastMove.x - p.x);
        dy = abs(_lastMove.y - p.y);
        _lastMove = p;
        if (_drawMode && ((dx > _moveth) || (dy > _moveth))) { //movement?
            fireEvent(_onTouchDraw, scr, EVENT::EVT_DRAW);
        }
        if (((now - _begin) > _clickLong) && (!_long)) {
            fireEvent(_onTouchLong, scr, EVENT::EVT_LONG);

            _long = true;
        }
    } else if (tch && !_down) { //start of touch
        scr = toScreen(p);
        _start = p;
        _lastMove = p;
        _begin = now;
        _long = false;
        _down = tch;

        fireEvent(_onTouchDown, scr, EVENT::EVT_DOWN);
    } else if (!tch && _down) { // stop of touch
        scr = toScreen(p);
        _down = tch;
        _long = false;
        fireEvent(_onTouchUp, scr, EVENT::EVT_UP);

        if (!_drawMode && ((abs(_start.x - p.x) > _wipeX) || (abs(_start.y - p.y) > _wipeY))) {
            if (_onTouchWipe)
                _onTouchWipe(wipeDirection(_start, p));

            fireAllEvents(scr, EVENT::EVT_WIPE);
        } else {
            if ((now - _begin) < _clickLong) {
                if ((now - _lastClick) < _dblClick) {
                    _lastClick = 0;
                    fireEvent(_onTouchDblClick, scr, EVENT::EVT_DBLCLICK);
                } else {
                    _lastClick = now;
                    fireEvent(_onTouchClick, scr, EVENT::EVT_CLICK);
                }
            }
        }
    }
}
//##################################################################

WIPE_DIRECTION ExtendedTouchEvent::wipeDirection(TS_Point b, TS_Point e) {
    TS_Point ptmp;
    int bx, by, ex, ey;
    ptmp = toScreen(b);
    bx = ptmp.x;
    by = ptmp.y;
    ptmp = toScreen(e);
    ex = ptmp.x;
    ey = ptmp.y;

    WIPE_DIRECTION dir; // wipe direction 0 = left, 1 = right, 2 = up, 3 = down
    if (abs(bx - ex) > abs(by - ey)) { // movement in x direction
        dir = ((bx - ex) > 0) ? WIPE_DIRECTION::WD_LEFT_RIGHT : WIPE_DIRECTION::WD_RIGHT_LEFT;
    } else { // movement in y direction
        dir = ((by - ey) > 0) ? WIPE_DIRECTION::WD_BOTTOM_UP : WIPE_DIRECTION::WD_TOP_DOWN;
    }
    return dir;
}
//##################################################################

void ExtendedTouchEvent::setResolution(int16_t xResolution, int16_t yResolution) {
    _xResolution = xResolution;
    _yResolution = yResolution;
}
//##################################################################

void ExtendedTouchEvent::setDrawMode(boolean drawMode) {
    _drawMode = drawMode;
}
//##################################################################

void ExtendedTouchEvent::calibrate(uint16_t smallMin, uint16_t longMin, uint16_t smallMax, uint16_t longMax) {
    _tsSmallMin = smallMin;
    _tsSmallMax = smallMax;
    _tsLongMin = longMin;
    _tsLongMax = longMax;
}
//##################################################################

void ExtendedTouchEvent::setMoveTreshold(uint16_t threshold) {
    _moveth = threshold;
}
//##################################################################

void ExtendedTouchEvent::setWipe(uint16_t wipeX, uint16_t wipeY) {
    _wipeX = wipeX;
    _wipeY = wipeY;
}
//##################################################################

void ExtendedTouchEvent::setLongClick(uint16_t clickLong) {
    _clickLong = clickLong;
}
//##################################################################

void ExtendedTouchEvent::setDblClick(uint16_t dblclick) {
    _dblClick = dblclick;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchDown(TS_Point_callback *callback) {
    _onTouchDown = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchUp(TS_Point_callback *callback) {
    _onTouchUp = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchClick(TS_Point_callback *callback) {
    _onTouchClick = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchDblClick(TS_Point_callback *callback) {
    _onTouchDblClick = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchLong(TS_Point_callback *callback) {
    _onTouchLong = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchDraw(TS_Point_callback *callback) {
    _onTouchDraw = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnTouchWipe(void (*callback)(WIPE_DIRECTION direction)) {
    _onTouchWipe = callback;
}
//##################################################################

void ExtendedTouchEvent::registerOnAllEvents(void (*callback)(TS_Point p, EVENT event)) {
    _onAllEvents = callback;
}
//##################################################################

void ExtendedTouchEvent::fireEvent(TS_Point_callback *callback, TS_Point scr, EVENT type) {
    if (callback) {
        callback(scr);
    }
    fireAllEvents(scr, type);
}
//##################################################################

void ExtendedTouchEvent::fireAllEvents(TS_Point scr, EVENT type) {
    if (_onAllEvents) {
        _onAllEvents(scr, type);
    }
}
//##################################################################

TS_Point ExtendedTouchEvent::toScreen(TS_Point p) {
    TS_Point screen;
    _lasttouch = millis();  // TODO auslagern?

    int w = _tft.width();
    int h = _tft.height();

    switch (_tft.getRotation()) {
        case 0:
            screen.x = map(p.x, _tsSmallMin, _tsSmallMax, 0, w);
            screen.y = map(p.y, _tsLongMin, _tsLongMax - _black, 0, h);
            break;
        case 1:
            screen.x = map(p.x, _tsLongMin, _tsLongMax - _black, 0, w);
            screen.y = map(p.y, _tsSmallMin, _tsSmallMax, 0, h);
            break;
        case 2:
            screen.x = map(p.x, _tsSmallMin, _tsSmallMax, 0, w);
            screen.y = map(p.y, _tsLongMin + _black, _tsLongMax, 0, h);
            break;
        case 3:
            screen.x = map(p.x, _tsLongMin + _black, _tsLongMax, 0, w);
            screen.y = map(p.y, _tsSmallMin, _tsSmallMax, 0, h);
            break;
    }

    return screen;
}
