#pragma once

#include <Arduino.h>

struct StatusInfo
{
  int value;
  const char *description;
};

void setupSensor();
uint8_t readSensorStatus();
const char *getStatusDescription(int value);
double measureTemperature(int averageOfMeasurements, int timeBetweenMeasurements);
