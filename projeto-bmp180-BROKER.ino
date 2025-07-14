#include <Wire.h>
#include <Adafruit_BMP085.h>

// --- Configurações de Wi-Fi e Adafruit IO ---
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define WIFI_SSID       "Wi-fi" 
#define WIFI_PASS       "Senha" 

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "Seu Username"     
#define AIO_KEY         "Sua Chave"

const int LED_GREEN_1 = 13;
const int LED_GREEN_2 = 32;
const int LED_YELLOW_1 = 14;
const int LED_YELLOW_2 = 27;
const int LED_RED_1 = 26;
const int LED_RED_2 = 25;
const int BUZZER_PIN = 23;
const int POTENTIOMETER_PIN = 34;

// --- Objetos do Sensor e MQTT ---
Adafruit_BMP085 bmp;

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temperaturaFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensor-bmp180.temperature");
Adafruit_MQTT_Publish pressaoFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensor-bmp180.pressure");
Adafruit_MQTT_Publish altitudeFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensor-bmp180.altitude");

// --- Funções de Conexão ---
void connectWiFi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Conectando ao Adafruit IO... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retentando conexão MQTT...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      Serial.println("Falha ao conectar ao Adafruit IO, travando!");
      while (1);
    }
  }
  Serial.println("Adafruit IO conectado!");
}

// --- Configuração dos Pinos ---
void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(LED_GREEN_1, OUTPUT);
  pinMode(LED_GREEN_2, OUTPUT);
  pinMode(LED_YELLOW_1, OUTPUT);
  pinMode(LED_YELLOW_2, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);


  connectWiFi(); // Conecta-se ao Wi-Fi
  if (!bmp.begin()) {
    Serial.println("Sensor BMP180 não encontrado. Verifique as conexões!");
    while (1);
  }

  Serial.println("Sensor BMP180 inicializado com sucesso!");
  Serial.println("------------------------------------");
}
// --- API do Código em Lógica ---
void loop() {
  connectMQTT();
  mqtt.processPackets(100);

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
  digitalWrite(LED_GREEN_1, LOW);
  digitalWrite(LED_GREEN_2, LOW);
  digitalWrite(LED_YELLOW_1, LOW);
  digitalWrite(LED_YELLOW_2, LOW);
  digitalWrite(LED_RED_1, LOW);
  digitalWrite(LED_RED_2, LOW);

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

    Serial.print("Valor do Potenciômetro: ");
    Serial.print(potValue);
    Serial.print(" -> Frequência do Buzzer: ");
    Serial.print(frequency);
    Serial.println(" Hz");

    tone(BUZZER_PIN, frequency);
    delay(200);
    noTone(BUZZER_PIN);
    delay(200);

  } else {
    noTone(BUZZER_PIN);
  }

  // --- Envia os dados para o Adafruit IO ---
  Serial.print("Enviando temperatura (");
  Serial.print(temperature);
  Serial.print(" *C)... ");
  if (! temperaturaFeed.publish(temperature)) {
    Serial.println("Falha ao enviar.");
  } else {
    Serial.println("Enviado com sucesso!");
  }

  Serial.print("Enviando pressao (");
  Serial.print(pressure);
  Serial.print(" Pa)... ");
  if (! pressaoFeed.publish(pressure)) {
    Serial.println("Falha ao enviar.");
  } else {
    Serial.println("Enviado com sucesso!");
  }

  Serial.print("Enviando altitude (");
  Serial.print(altitude);
  Serial.print(" m)... ");
  if (! altitudeFeed.publish(altitude)) {
    Serial.println("Falha ao enviar.");
  } else {
    Serial.println("Enviado com sucesso!");
  }
delay(15000);
}