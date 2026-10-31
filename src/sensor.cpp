#include "sensor.h"
#include "MAX6675.h"

const int selectPin = 5;

MAX6675 thermoCouple(selectPin, &SPI);

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

void setupSensor()
{
  SPI.begin();
  thermoCouple.begin();

  uint8_t status = thermoCouple.read();
  if (status != 0)
    Serial.println(status);

  thermoCouple.setSPIspeed(4000000);
  thermoCouple.setOffset(0);
}

uint8_t readSensorStatus()
{
  return thermoCouple.read();
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
