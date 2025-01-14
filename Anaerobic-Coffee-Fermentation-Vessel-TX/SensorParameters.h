//#define TdsSensorPin A0

// Define the pH level pin and temperature pin
#define PH4502C_PH_LEVEL_PIN A1
#define PH4502C_TEMP_PIN A2

#define ONE_WIRE_BUS 5


// Create an instance of the PH4502C_Sensor
PH4502C_Sensor ph4502(PH4502C_PH_LEVEL_PIN, PH4502C_TEMP_PIN);
//GravityTDS gravityTds;

//float tdsValue = 0;

SHTSensor m5(SHTSensor::SHT3X);

Adafruit_AHTX0 aht20;

OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature dallas_sensors(&oneWire);

sensors_event_t aht20_humidity, aht20_temp;


#define TdsSensorPin A0
#define VREF 5.0   // analog reference voltage(Volt) of the ADC
#define SCOUNT 30  // sum of sample point

int analogBuffer[SCOUNT];  // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float tds_temperature = 16;  // current temperature for compensation