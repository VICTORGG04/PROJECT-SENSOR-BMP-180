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

#define WIFI_SSID       "NPITI-IoT" 
#define WIFI_PASS       "NPITI-IoT" 

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "VICTORGG04X"     
#define AIO_KEY         "aio_vFdY76rmtkWifggeNQE2xeihFucB"

#define ledVerd         13
#define ledVerdDOIS        12
#define ledAmar         14
#define ledAmarDOIS        27
#define ledVerm         26
#define ledVermDOIS        25
const int  buzzer = 23;
int pot = 34;

// --- Objetos do Sensor e MQTT ---
Adafruit_BMP085 bmp; // Objeto para o sensor BMP180

WiFiClient client; // Objeto cliente Wi-Fi

// Configuração do cliente MQTT com o servidor, porta, username e chave AIO
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Configuração dos feeds para publicar dados
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
  pinMode(ledVerd, OUTPUT);
  pinMode(ledAmar, OUTPUT);
  pinMode(ledVerm, OUTPUT);
  pinMode(ledVerdDOIS, OUTPUT);
  pinMode(ledAmarDOIS, OUTPUT);
  pinMode(ledVermDOIS, OUTPUT);
  pinMode(buzzer, OUTPUT);


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
  connectMQTT(); // Garante que a conexão MQTT está ativa
  mqtt.processPackets(100);

  // Leitura da Temperatura
  float temperature = bmp.readTemperature();
  Serial.print("Temperatura = ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Leitura da Pressão (em Pascais)
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
  if(pressure > 100500 && pressure < 101500){
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, LOW);
    digitalWrite(ledAmar, LOW);
    digitalWrite(ledAmarDOIS, LOW);
    digitalWrite(ledVerm, LOW);
    digitalWrite(ledVermDOIS, LOW);
  } else if(pressure > 101500 && pressure < 102000){
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, LOW);
    digitalWrite(ledAmarDOIS, LOW);
    digitalWrite(ledVerm, LOW);
    digitalWrite(ledVermDOIS, LOW);
  } else if(pressure > 102000 && pressure < 102250){
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, LOW);
    digitalWrite(ledVerm, LOW);
    digitalWrite(ledVermDOIS, LOW);
  } else if(pressure > 102500 && pressure < 102750){
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, HIGH);
    digitalWrite(ledVerm, LOW);
    digitalWrite(ledVermDOIS, LOW);
  } else if(pressure > 102750 && pressure < 103000){
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, HIGH);
    digitalWrite(ledVerm, HIGH);
    digitalWrite(ledVermDOIS, LOW);
  } else if(pressure > 103000 && pressure < 103250){
    digitalWrite(ledVerd, HIGH);
    digitalWrite(ledVerdDOIS, HIGH);
    digitalWrite(ledAmar, HIGH);
    digitalWrite(ledAmarDOIS, HIGH);
    digitalWrite(ledVerm, HIGH);
    digitalWrite(ledVermDOIS, HIGH);
  }

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
    noTone(buzzer); // Garante que o buzzer esteja desligado se a pressão for menor
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

}