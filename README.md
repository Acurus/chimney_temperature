# Overview
Arduino skectch for reading temperature with a K type Thremocouple and sending it over MQTT.

This is a [PlatformIO](https://platformio.org/) project. Open the folder in VS Code with the PlatformIO IDE extension, or use the CLI:

```
pio run                 # build
pio run -t upload       # flash the NodeMCU
pio device monitor      # serial monitor (115200 baud)
```

PlatformIO Core runs from `~/.platformio/penv`. To build that environment on a uv-managed Python, so it doesn't break when the system Python is upgraded:

```
uv python install 3.11
uv venv --python 3.11 --python-preference only-managed --seed ~/.platformio/penv
VIRTUAL_ENV=~/.platformio/penv uv pip install platformio
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
