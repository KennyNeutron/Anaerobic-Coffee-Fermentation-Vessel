# ExtendedTouchEvent
An ARDUINO library to poll touchscreens based on XPD 2046 controller and call functions on certain events.

It is derived from the library [TouchEvent](https://github.com/GerLech/TouchEvent) by Gerald Lechner, making some enhancements in performance and usability - no need for negative values and fancy switch cases ;-) or manual updates if you decide to alter the orientation of your panel while running your sketch - just a simple call of **setResolution()** once in setup() and after calling **setRotation()** the coordinates of the touch will be calculated according to the choosen orientation.
The functions isIn*() and setRotation() are covered by tests performed on a D1_MINI resp. ESP32 DEV. 

To init the class you need to pass a pointer of instances of XPT2046\_Touchscreen and Adafruit\_GFX.

Put a call to the pollTouchScreen() function of this library into the main loop. This function keeps track about touch position and touch state. For all registered callbacks, the corresponding functions will be called on the following events:

- **TouchDown** Touch screen has been touched. The callback function gets the position in the displays coordinates.
- **TouchUp**   Touch screen is no longer touched. The callback function gets the position in the displays coordinates.
- **TouchClick** Touch screen was touched and released. The callback function gets the position in the displays coordinates.
- **TouchDblClick** Touch screen was touched and released twice in a short time. The callback function gets the position in the displays coordinates.
- **TouchLongClick** Touch screen was touched for a longer time before released. The callback function gets the position in the displays coordinates.
- **TouchDraw** Touchpoint was moved while touched. This event is only active if draw mode is true. The callback function gets the position in the displays coordinates.
- **TouchWipe** Touch was moved a certain distance while touched. This event is only active if draw mode is false. The callback function gets the direction of the movement.

All required parameters will be initialized with common valid default values and can be changed at run time.

**Reference to all defined funktions:**

- **void pollTouchScreen()**
  this is the main function which should be called in the main loop
  positions will be checked and under certain conditions events will be triggered


Initialization and customizing can be achieved with th following methods
- **void setResolution(int16_t xResolution, int16_t yResolution)**
  the number of pixels for the used TFT display default is w=240 h=320, call once during setup()
- **void calibrate(uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax)**
  values to define the top left and the bottom right values returned from the touch screen. 
  default values are for for a 2.4" TFT display with 320 x 240 pixels
- **void setDrawMode(boolean drawMode)**
  Switches draw mode on or off. Move events will be triggered only if draw mode is true, wipe events if it is false
- **void setMoveTreshold(uint16_t threshold)**
  define the minimal difference to the last point to detect a move
  default = 10
- **void setWipe(uint16_t wipeX, uint16_t wipeY)**
  define the minimal distance between touch start- and endpoint to detect a wipe event defaults = 500 and 700
- **void setLongClick(uint16_t clickLong)**
  define the minimal time in ms to interpret a click as a long click, default = 1000 ms
- **void setDblClick(uint16_t dblclick)**
  sets the minimal time (in ms) between two clicks to be recognized as a doubleclick, defaults to 500ms 

Useful functions
- **boolean isInRectangle(TS_Point p, int16_t x, int16_t y, int16_t w, int16_t h)**
  function to detect if a position is inside a rectangle starting at (x, y) with width w and height h. Helps to detect if a 
  touch hits a button
- **boolean isInCircle(TS_Point p, int16_t x, int16_t y, int16_t r)** 
  checks whether a Point *p* is inside a circle with centre at (*x, y*) and radius 
- **boolean isInPolygon(TS_Point p, int16_t nvertices, int16_t vertices[][2])**
  checks whether a Point *p* is inside a polygon, which is defined by the number vertices *nvertices* and a corresponding array with the pairs of x- and y-coordinates of each vertex. Only simple convex or concave polygons are supported, for an explanation of these terms please have a look at https://en.wikipedia.org/wiki/Polygon. Order of the vertices in the array must be clockwise or counterclockwise.
- **unsigned long lastTouchEventTime()**
  returns the time (in ms) since the last touch event occured. Use for decisions like starting screensaver or sleep of controler 
- **void setRotation(uint8_t n)**
  sets the orientation of tft and touchscreen (turns the display). Each call ist delegated to the methods of the same name from  XPT2046\_Touchscreen and Adafruit\_GFX

Most callback functions have a signature of void (TS\_Point\_callback)(TS\_Point p) (accept a TS\_Point, return nothing). Before use they have to be registered - typically once during setup of the sketch although it's possible too to register or unregister later.
- **void registerOnTouchDown(TS_Point_callback * callback)**
  register a callback function for touch down start event
- **void registerOnTouchUp(TS_Point_callback * callback)**
  register a callback function for touch up event
- **void registerOnTouchClick(TS_Point_callback * callback)**
  register a callback function for click event
- **void registerOnTouchDblClick(TS_Point_callback * callback))**
  register a callback function for doubleclick event
- **void registerOnTouchLong(TS_Point_callback * callback)**
  register a callback function for touch long click event
- **void registerOnTouchDraw(TS_Point_callback * callback)**
  register a callback function for touch draw event
- **void registerOnTouchWipe(void (\* callback)(WIPE_DIRECTION direction))**
  WIPE_DIRECTION is a typesafe enum representing the four possible movements
- **void registerOnAllEvents(void (\* callback)(TS_Point p, EVENT event))**
  registers a callback for any event - if you prefer a more centralized handler, EVENT ist a typesafe enum describing the type of
  the event

