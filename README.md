# Overview
Arduino skectch for reading temperature with a K type Thremocouple and sending it over MQTT.

create an arduino_secrets.h file with the following content: 

```
#define WIFI_SSID 
#define WIFI_PASSWORD 
#define MQTT_BROKER 
#define MQTT_PORT 
#define MQTT_USERNAME 
#define MQTT_PASSWORD 
```

# Hardware
* ESP8266 (NodeMCU V3)
* MAX6675 Module + K Type Thermocouple

# Wiring
MAX6675 SO (Signal out) -> ESP8266 D6
MAX6675 CS (Chip Select) -> ESP8266 D1
MAX6675 CLK (Clock) -> ESP8266 D5
MAX6675 VCC (Voltage) -> ESP8266 3V3
MAX6675 GND (Ground) -> ESP8266 GND
