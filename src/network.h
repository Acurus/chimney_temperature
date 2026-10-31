#pragma once

void connectWifi();
void connectMqtt();
void ensureMqttConnected();
void publishTemperature(double temperature);
void pollMqtt();
