# everblu-meters
Fetch water usage data from Cyble EverBlu meters using RADIAN protocol.


## Hardware
![Raspberry Pi Zero with CC1101](board.jpg)
Currently the project runs on Raspberry Pi with an RF transreciver (CC1101). 

### Connections (rpi to CC1101):
- pin 1 (3V3) to pin 2 (VCC)
- pin 6 (GND) to pin 1 (GND)
- pin 11 (GPIO0	) to pin 3 (GDO0)
- pin 24 (CE0) to pin 4 (CSN)
- pin 23 (SCLK) to pin 5 (SCK)
- pin 19 (MOSI) to pin 6 (MOSI)
- pin 21 (MISO) to pin 7 (MISO)
- pin 13 (GPIO27) to pin 8 (GD02)


## Configuration
1. Enable SPI in raspi-config.
2. Install WiringPi from https://github.com/WiringPi/WiringPi/
3. Set meter serial number and production date in `cc1101.c`, it can be found on the meter label itself:
![Cyble Meter Label](meter_label.png)




## Compiling

```
gcc radian_trx.c -o radian_trx -lwiringPi -lpthread -Wall
```

## Troubleshooting

### Frequency adjustment



## Origin and license

This code is fetched from http://www.lamaisonsimon.fr/wiki/doku.php?id=maison2:compteur_d_eau:compteur_d_eau 



The license is unknown, citing one of the authors (fred):

> I didn't put a license on this code maybe I should, I didn't know much about it in terms of licensing.
> this code was made by "looking" at the radian protocol which is said to be open source earlier in the page, I don't know if that helps?




