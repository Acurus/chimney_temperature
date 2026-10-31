#include <Arduino.h>
#include "sensor.h"
#include "network.h"

const int SENSOR_READ_DELAY = 500;
const int AVERAGE_COUNT = 4;
const unsigned long MEASUREMENT_INTERVAL = 58000; // ms

double temperature = 0;

void setup()
{
  Serial.begin(115200);

  setupSensor();
  connectWifi();
  connectMqtt();
}

unsigned long lastTemperatureCheck = 0;

void loop()
{
  ensureMqttConnected();

  unsigned long currentMillis = millis();
  if ((currentMillis - lastTemperatureCheck) >= MEASUREMENT_INTERVAL)
  {
    lastTemperatureCheck = currentMillis;

    uint8_t status = readSensorStatus();
    if (status == 0)
    {
      temperature = measureTemperature(AVERAGE_COUNT, SENSOR_READ_DELAY);

      if (!isnan(temperature))
      {
        Serial.print("Temperature = ");
        Serial.println(temperature);

        publishTemperature(temperature);
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

  pollMqtt();
}
