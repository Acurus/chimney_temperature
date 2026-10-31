#include "network.h"
#include <ESP8266WiFi.h>
#include <ArduinoMqttClient.h>
#include "sensor.h"
#include "arduino_secrets.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *mqtt_broker = MQTT_BROKER;
int mqtt_port = MQTT_PORT;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;
const char topic[] = "chimney_temperature";

StatusInfo wifiStatusTable[] = {
    {WL_IDLE_STATUS, "Idle"},
    {WL_NO_SSID_AVAIL, "SSID not found"},
    {WL_SCAN_COMPLETED, "Scan completed"},
    {WL_CONNECTED, "Connected"},
    {WL_CONNECT_FAILED, "Connection failed"},
    {WL_CONNECTION_LOST, "Connection lost"},
    {WL_WRONG_PASSWORD, "Wrong password"},
    {WL_DISCONNECTED, "Disconnected"}};

const char *getWifiStatusDescription(int value)
{
  for (int i = 0; i < sizeof(wifiStatusTable) / sizeof(StatusInfo); i++)
  {
    if (wifiStatusTable[i].value == value)
    {
      return wifiStatusTable[i].description;
    }
  }
  return "Unknown status";
}

void connectWifi()
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("WiFi status ");
    Serial.print(WiFi.status());
    Serial.print(": ");
    Serial.println(getWifiStatusDescription(WiFi.status()));
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt()
{
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(mqtt_broker);
  mqttClient.setUsernamePassword(mqtt_username, mqtt_password);
  while (!mqttClient.connect(mqtt_broker, mqtt_port))
  {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(500);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void ensureMqttConnected()
{
  if (!mqttClient.connected())
  {
    Serial.println("MQTT disconnected. Attempting to reconnect...");
    while (!mqttClient.connect(mqtt_broker, mqtt_port))
    {
      delay(1000);
    }
  }
}

void publishTemperature(double temperature)
{
  mqttClient.beginMessage(topic);
  mqttClient.print(temperature);
  mqttClient.endMessage();
}

void pollMqtt()
{
  mqttClient.poll(); // Handle MQTT messages and keep the connection alive
}
