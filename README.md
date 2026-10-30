# Overview
Arduino skectch for reading temperature with a K type Thremocouple and sending it over MQTT.

This is a [PlatformIO](https://platformio.org/) project. Open the folder in VS Code with the PlatformIO IDE extension, or use the CLI:

```
pio run                 # build
pio run -t upload       # flash the NodeMCU
pio device monitor      # serial monitor (115200 baud)
```

Copy `include/arduino_secrets.h.example` to `include/arduino_secrets.h` and fill in your values. It needs the following content:

```
#define WIFI_SSID 
#define WIFI_PASSWORD 
#define MQTT_BROKER 
#define MQTT_PORT 
#define MQTT_USERNAME 
#define MQTT_PASSWORD 
```

# Libraries
Installed automatically by PlatformIO via `lib_deps` in `platformio.ini`.

* [ArduinoMqttClient](https://github.com/arduino-libraries/ArduinoMqttClient)

* [MAX6675](https://github.com/RobTillaart/MAX6675)

# Hardware
* ESP8266 (NodeMCU V3)
* MAX6675 Module + K Type Thermocouple

# Wiring
| MAX66775 PIN | Description | ESP8266 PIN |
|--------------|-------------|-------------|
| SO           |Signal out   |  D6         |
| CS           |Chip Select  |  D1         |
| CLK          |Clock        | D5          |
| VCC          |Voltage      | 3V3         |
| GND          |Ground       |  GND        |
