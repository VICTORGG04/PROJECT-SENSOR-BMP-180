#include <Wire.h> 
#include <Adafruit_BMP085.h>

#define ledVerd 13
#define ledVerdDOIS 12
#define ledAmar 14
#define ledAmarDOIS 27
#define ledVerm 26
#define ledVermDOIS 25
const int buzzer = 23;
int pot = 34; 

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);

  pinMode(ledVerd, OUTPUT);
  pinMode(ledVerdDOIS, OUTPUT);
  pinMode(ledAmar, OUTPUT);
  pinMode(ledAmarDOIS, OUTPUT);
  pinMode(ledVerm, OUTPUT);
  pinMode(ledVermDOIS, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Verifica se o sensor foi encontrado
  if (!bmp.begin()) {
    Serial.println("Sensor BMP180 não encontrado. Verifique as conexões!");
    while (1); // Trava o programa se o sensor não for encontrado
  }

  Serial.println("Sensor BMP180 inicializado com sucesso!");
  Serial.println("------------------------------------");
}

void loop() {
  float temperature = bmp.readTemperature();
  Serial.print("Temperatura = ");
  Serial.print(temperature);
  Serial.println(" *C");

  long pressure = bmp.readPressure();
  Serial.print("Pressao = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  float altitude = bmp.readAltitude(101325);
  Serial.print("Altitude aproximada = ");
  Serial.print(altitude);
  Serial.println(" metros");

  Serial.println("------------------------------------");

  // --- Lógica dos LEDs baseada na pressão ---
  digitalWrite(ledVerd, LOW);
  digitalWrite(ledVerdDOIS, LOW);
  digitalWrite(ledAmar, LOW);
  digitalWrite(ledAmarDOIS, LOW);
  digitalWrite(ledVerm, LOW);
  digitalWrite(ledVermDOIS, LOW);

  if (pressure > 100500 && pressure < 101500) {
    digitalWrite(ledVerd, HIGH);
  } else if (pressure >= 101500 && pressure < 102000) {
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
  } else if (pressure >= 102000 && pressure < 102250) {
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
  } else if (pressure >= 102500 && pressure < 102750) {
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, HIGH);
  } else if (pressure >= 102750 && pressure < 103000) {
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, HIGH);
    digitalWrite(ledVerm, HIGH);
  } else if (pressure >= 103000 && pressure < 103250) {
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, HIGH);
    digitalWrite(ledVerm, HIGH);
    digitalWrite(ledVermDOIS, HIGH);
  }

  // --- Lógica do Buzzer controlada pelo potenciômetro ---
  if (pressure > 103000) {
    int potValue = analogRead(pot);
    int frequency = map(potValue, 0, 4095, 100, 2000);

    Serial.print("Valor do Potenciômetro: ");
    Serial.print(potValue);
    Serial.print(" -> Frequência do Buzzer: ");
    Serial.print(frequency);
    Serial.println(" Hz");

    tone(buzzer, frequency);
    delay(200);
    noTone(buzzer);
    delay(200);
  } else {
    noTone(buzzer);
  }
  delay(1000);
}
