#include <Wire.h>
#include <Adafruit_BMP085.h>

const int LED_GREEN_1 = 13;
const int LED_GREEN_2 = 32;
const int LED_YELLOW_1 = 14;
const int LED_YELLOW_2 = 27;
const int LED_RED_1 = 26;
const int LED_RED_2 = 25;
const int BUZZER_PIN = 23;
const int POTENTIOMETER_PIN = 34;

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);

  pinMode(LED_GREEN_1, OUTPUT);
  pinMode(LED_GREEN_2, OUTPUT);
  pinMode(LED_YELLOW_1, OUTPUT);
  pinMode(LED_YELLOW_2, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!bmp.begin()) {
    Serial.println("BMP180 Sensor not found. Please check connections!");
    while (true); 
  }

  Serial.println("BMP180 Sensor initialized successfully!");
  Serial.println("------------------------------------");
}

void loop() {
  float temperature = bmp.readTemperature();
  long pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(101325); 

  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print("Approximate Altitude = ");
  Serial.print(altitude);
  Serial.println(" meters");

  Serial.println("------------------------------------");

  // --- LED Logic based on Pressure ---
  digitalWrite(LED_GREEN_1, LOW);
  digitalWrite(LED_GREEN_2, LOW);
  digitalWrite(LED_YELLOW_1, LOW);
  digitalWrite(LED_YELLOW_2, LOW);
  digitalWrite(LED_RED_1, LOW);
  digitalWrite(LED_RED_2, LOW);

  // Use a switch-like structure with if-else if for pressure ranges
  if (pressure >= 101300 && pressure < 101500) {
    digitalWrite(LED_GREEN_1, HIGH);
  } else if (pressure >= 101500 && pressure < 102000) {
    digitalWrite(LED_GREEN_1, HIGH);
    digitalWrite(LED_GREEN_2, HIGH);
  } else if (pressure >= 102000 && pressure < 102250) {
    digitalWrite(LED_GREEN_1, HIGH);
    digitalWrite(LED_GREEN_2, HIGH);
    digitalWrite(LED_YELLOW_1, HIGH);
  } else if (pressure >= 102500 && pressure < 102750) {
    digitalWrite(LED_GREEN_1, HIGH);
    digitalWrite(LED_GREEN_2, HIGH);
    digitalWrite(LED_YELLOW_1, HIGH);
    digitalWrite(LED_YELLOW_2, HIGH);
  } else if (pressure >= 102750 && pressure < 103000) {
    digitalWrite(LED_GREEN_1, HIGH);
    digitalWrite(LED_GREEN_2, HIGH);
    digitalWrite(LED_YELLOW_1, HIGH);
    digitalWrite(LED_YELLOW_2, HIGH);
    digitalWrite(LED_RED_1, HIGH);
  } else if (pressure >= 103000 && pressure < 103250) {
    digitalWrite(LED_GREEN_1, HIGH);
    digitalWrite(LED_GREEN_2, HIGH);
    digitalWrite(LED_YELLOW_1, HIGH);
    digitalWrite(LED_YELLOW_2, HIGH);
    digitalWrite(LED_RED_1, HIGH);
    digitalWrite(LED_RED_2, HIGH);
  }

  // --- Buzzer Logic controlled by Potentiometer ---
  if (pressure > 103000) {
    int potValue = analogRead(POTENTIOMETER_PIN);
    int frequency = map(potValue, 0, 4095, 100, 2000);

    Serial.print("Potentiometer Value: ");
    Serial.print(potValue);
    Serial.print(" -> Buzzer Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");

    tone(BUZZER_PIN, frequency);
    delay(200); 
    noTone(BUZZER_PIN);
    delay(200);
  } else {
    noTone(BUZZER_PIN);
  }
  delay(1000);
}
