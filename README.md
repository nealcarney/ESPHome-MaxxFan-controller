# MaxxFan and air conditioner controller

This project has two samples, both use an ESP32-based controller to control a MaxxFan using ESPHome and Home Assistant. The original solution also can control a Houghton air conditioner (or any AC using IR remote).
![Preview](https://github.com/nealcarney/ESPHome-MaxxFan-controller/assets/131406498/6c176993-6056-4b33-92c8-b6360162dcb2)

## MaxxFan Features

### MaxxFan only

- Control MaxxFan by emulating the wall-mount remote control which you connect via RJ-45.
- Toggle for power on/off, hood open/close, and direction intake/exhaust. Slider to control fan speed from 10% to 100%.

### MaxxFan and Houghton AC

- Temperature and humidity sensor.
- Overall power consumption measurement to facilitate monitoring. Also added board voltage measurement to facilitate trouble alerts.
- ‘Resynchronize’ feature in case HA gets out of sync with fan. If someone manually presses the MaxxFan control panel, HA won’t know about it and will be out of sync. Pressing the resync button will send an IR command to set the MaxxFan at a predetermined state; will also adjust UI and tracking logic in HA to same state.
- Keeps track of uptime, firmware status, and allows for reset.

## AC/heatpump features

- Controls the the AC/heatpump using IR transmitter to send commands to set heat/cool mode, fan speed, and power on/off. Temperature setting explained below.  The IR codes embedded in my ESPHome code is specific to Houghton AC, but you can substitute with appropriate codes for your particular AC unit.
- One of the downsides of most rooftop AC units is that the temperature sensor is located in the return air duct in the middle of the ceiling control panel. That is simply too close to the conditioned air output, especially since the air exiting the side and rear air vents immediately bounces off the wall and back toward the return air duct. So the built-in sensor gets inaccurate readings of actual room temperature, and the units tend to short-cycle. That definitely is the case with mine prior to this modification.
- So rather than use the built-in thermostat and let the unit determine on-off state, my controller takes over the temperature monitoring and on/off logic. It sends an ON command at max temp when the actual room temp is outside of desired temp band, and an OFF command once target temp is achieved. It only needs to use two temperatures: max low is 60F and max high is 85F.

## Known issues

The schematics and PCB are a prototype!!!  Please do not try to use as-is.  Later I will update with more info on what I had to modify to make v1.0 work, and what has changed with v1.1 which has not been tested.

## Links

- [PCB design files](https://oshwlab.com/ncarney/maxxfan-controller)
- [Youtube demo of functionality](https://youtu.be/xNdgwMG5jzs)
- [Post on Ford Transit USA Forum with more Home Assistant info](https://www.fordtransitusaforum.com/threads/home-assistant-for-van-automation.90215/post-1245570)
- [Post on Ford Transit USA Forum with more MaxxFan Houghton Controller discussion](https://www.fordtransitusaforum.com/threads/maxxfan-houghton-controller-for-home-assistant.94841/post-1245938)
- [AHT21 housing on Thingiverse](https://www.thingiverse.com/thing:5200358)

## Repository Structure

- `/homeassistant/` - Contains Home Assistant and ESPHome configuration files
  - `/esphome/` - ESPHome configuration and custom components
    - `maxxfan.yaml` - Full configuration with MaxxFan and AC control
    - `maxxfan-lite.yaml` - Simplified configuration for MaxxFan only
    - `custom_outputs.h` - Custom C++ code for MaxxFan control
  - `/www/` - Web assets for Lovelace UI customization
- `/MaxxFan-Controller-Lite/` - Contains the MaxxFan-Controller-Lite hardware files designed in Fusion 360
  - You can order the [MaxxFan only boards on PCBWay](https://www.pcbway.com/project/shareproject/W829927AS1Y4_WALTERS_schematic_v24_2025_01_10_53f183b7.html)
- And the PCB design files for the MaxxFan + Houghton AC are hosted separately on [OSH Lab](https://oshwlab.com/ncarney/maxxfan-controller)

## Notes

- The homeassistant folder contains a www folder.  This contains the dependencies needed for some of the Lovelace UI elements that I used.  If interested you can install in your own www folder and then add to the resources section of your dashboard management.
