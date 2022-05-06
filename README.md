# everblu-meters - Water usage data for Home Assistant
Fetch water/gas usage data from Cyble EverBlu meters using RADIAN protocol on 433Mhz. Intergrated with Home Assistant via MQTT. 

Note: HASS autodiscovery is still missing, during development.

Meters supported:
- Itron EverBlu Cyble Enhanced


## Hardware
![Raspberry Pi Zero with CC1101](board.jpg)
The project runs on ESP8266/ESP32 with an RF transreciver (CC1101). 

### Connections (ESP32 to CC1101):
- GND to pin 1 (GND)
- 3V3 to pin 2 (VCC)
- GPIO0 to pin 3 (GDO0)
- GPIO5 (CS) to pin 4 (CSN)
- GPIO18 (CLK) to pin 5 (SCK)
- GPIO23 (MOSI) to pin 6 (MOSI)
- GPIO19 (MISO) to pin 7 (MISO)
- GPIO2 to pin 8 (GD02)


## Configuration
1. Install EasyMQTTClient through Arduino library manager as it required for MQTT
2. Update WiFi and MQTT details in everblu-meters-esp8266.ino, if you do not use username and password for MQTT then comment those out with //
3. Set meter serial number (without the leading 0) and production year in `everblu_meters.h` (at the end of the file), it can be found on the meter label itself:
![Cyble Meter Label](meter_label.png)
4. Flash the sketch to your ESP device
5. After a few second your meter data should be on the screen (serial console) and data should be pushed to MQTT.
6. The device will query the meter twice a day, every 12 hours.

## Troubleshooting

### Frequency adjustment
Your transreciver module may be not calibrated correctly, please modify frequency a bit lower or higher and try again. You may use RTL-SDR to measure the offset needed.

### Business hours
Your meter may be configured in such a way that is listens for request only during hours when data collectors work - to conserve energy. If you are unable to communicate with the meter, please try again during business hours (8-16).

### Serial number starting with 0
Please ignore the leading 0, provide serial in configuration without it.


## Origin and license

This code is based on code from http://www.lamaisonsimon.fr/wiki/doku.php?id=maison2:compteur_d_eau:compteur_d_eau 


The license is unknown, citing one of the authors (fred):

> I didn't put a license on this code maybe I should, I didn't know much about it in terms of licensing.
> this code was made by "looking" at the radian protocol which is said to be open source earlier in the page, I don't know if that helps?


