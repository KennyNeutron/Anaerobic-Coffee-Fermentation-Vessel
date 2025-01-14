// Define pin for the MG811 sensor
const int analogPin = A0;  // Analog pin connected to AOUT of the MG811

// Calibration constants (replace with your measured values)
float voltage400ppm = 0.31;  // Example voltage for 400 ppm (baseline in fresh air)
float voltage1000ppm = 0.45; // Example voltage for 1000 ppm (high CO2 environment)

// Calculate slope and intercept for the linear relationship
float slope = (1000.0 - 400.0) / (voltage1000ppm - voltage400ppm); // Δppm / Δvoltage
float intercept = 400.0 - slope * voltage400ppm;                  // Calculate intercept

void setup() {
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
}

void loop() {
  // Read the analog value from the sensor
  int rawValue = analogRead(analogPin);
  
  // Convert to voltage (assuming 5V ADC and 10-bit resolution)
  float voltage = rawValue * (5.0 / 1023.0);

  // Calculate CO2 concentration using the calibration data
  float co2_concentration = slope * voltage + intercept;

  // Print values to Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, CO2 Concentration: ");
  Serial.print(co2_concentration);
  Serial.println(" ppm");

  // Delay for stability
  delay(1000);
}
