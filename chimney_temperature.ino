#include <ESP8266WiFi.h>
#include <ArduinoMqttClient.h>
#include "MAX6675.h"
#include "arduino_secrets.h"

const int selectPin = 5;
const int SENSOR_READ_DELAY = 500;
const int AVERAGE_COUNT = 4;
const unsigned long MEASUREMENT_INTERVAL = 58000; // ms

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
MAX6675 thermoCouple(selectPin, &SPI);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *mqtt_broker = MQTT_BROKER;
int mqtt_port = MQTT_PORT;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;
const char topic[] = "chimney_temperature";
double temperature = 0;



struct StatusInfo
{
  int value;
  const char *description;
};

StatusInfo statusTable[] = {
    {0, "OK"},
    {4, "Thermocouple short to VCC"},
    {128, "No read done yet"},
    {129, "No communication"}};

const char *getStatusDescription(int value)
{
  for (int i = 0; i < sizeof(statusTable) / sizeof(StatusInfo); i++)
  {
    if (statusTable[i].value == value)
    {
      return statusTable[i].description;
    }
  }
  return "Unknown status";
}

void setup()
{
  Serial.begin(115200);

  SPI.begin();
  // Setup sensor
  thermoCouple.begin();

  uint8_t status = thermoCouple.read();
  if (status != 0)
    Serial.println(status);

  thermoCouple.setSPIspeed(4000000);
  thermoCouple.setOffset(0);

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

  while (WiFi.status() != WL_CONNECTED)
  {
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
  while (!mqttClient.connect(mqtt_broker, mqtt_port))
  {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(500);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

unsigned long lastTemperatureCheck = 0;

void loop()
{
  if (!mqttClient.connected())
  {
    Serial.println("MQTT disconnected. Attempting to reconnect...");
    while (!mqttClient.connect(mqtt_broker, mqtt_port))
    {
      delay(1000);
    }
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - lastTemperatureCheck >= MEASUREMENT_INTERVAL);
  {
    lastTemperatureCheck = currentMillis;

    uint8_t status = thermoCouple.read();
    if (status == 0)
    {
      temperature = measureTemperature(AVERAGE_COUNT, SENSOR_READ_DELAY);

      if (!isnan(temperature))
      {
        Serial.print("Temperature = ");
        Serial.println(temperature);

        mqttClient.beginMessage(topic);
        mqttClient.print(temperature);
        mqttClient.endMessage();
      }
      else
      {
        Serial.println("Invalid temperature measurement.");
      }
    }
    else
    {
      Serial.print("Sensor error: ");
      Serial.println(getStatusDescription(status));
    }
  }

  mqttClient.poll(); // Handle MQTT messages and keep the connection alive
}

double measureTemperature(int averageOfMeasurements, int timeBetweenMeasurements)
{
  double sum = 0;
  int validReadings = 0;

  for (int i = 0; i < averageOfMeasurements; i++)
  {
    uint8_t status = thermoCouple.read();
    if (status == 0) // Only add valid readings
    {
      sum += thermoCouple.getTemperature();
      validReadings++;
    }
    else
    {
      Serial.print("Skipping measurement due to error: ");
      Serial.println(getStatusDescription(status));
    }
    delay(timeBetweenMeasurements);
  }

  if (validReadings > 0)
    return (sum / validReadings);
  else
    return NAN; // Return NaN if no valid readings were taken
}
