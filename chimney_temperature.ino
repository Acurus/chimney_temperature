#include <ESP8266WiFi.h>
#include <ArduinoMqttClient.h>
#include "MAX6675.h"
#include "arduino_secrets.h"


#define dataPin 16
#define selectPin 5
#define clockPin 4

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
MAX6675 thermoCouple(selectPin, dataPin, clockPin);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_broker = MQTT_BROKER;
int mqtt_port = MQTT_PORT;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;
const char topic[] = "chimney_temperature";
double temperature = 0;

void setup() {
  Serial.begin(115200);

  // Setup sensor
  thermoCouple.begin();

  uint8_t status = thermoCouple.read();
  if (status != 0) Serial.println(status);
  if (thermoCouple.getRawData() == 0xFFFF) {
    Serial.println("NO COMMUNICATION");
  }

  thermoCouple.setSPIspeed(4000000);
  thermoCouple.setOffset(0);
  temperature = thermoCouple.getTemperature();


  // Connecting to WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(mqtt_broker);
  mqttClient.setUsernamePassword(mqtt_username, mqtt_password);
  while (!mqttClient.connect(mqtt_broker, mqtt_port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(500);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  temperature = measureTemperature(10000, 500);

  Serial.print("Temperature = ");
  Serial.println(temperature);

  mqttClient.beginMessage(topic);
  mqttClient.print(temperature);
  mqttClient.endMessage();

  delay(1000);
}

double measureTemperature(int measureTimeMs, int delayTimeMs) {
  int timer = 0;
  double minTemp = 10000;
  double temperature;
  while (timer < measureTimeMs)
  {
    temperature = thermoCouple.getTemperature();
    if (temperature < minTemp) {
      minTemp = temperature;
    }
    delay(delayTimeMs);
    timer += (delayTimeMs);
  }

  return minTemp;
}
