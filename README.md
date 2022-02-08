# everblu-meters
Fetch water usage data from Cyble EverBlu meters using RADIAN protocol.


## Hardware
Currently the project runs on Raspberry Pi with an RF transreciver (CC1101).


## Configuration
You will need serial number of the meter, it can be found on the meter label itself 
![Cyble Meter Label](meter_label.png)



```
CC1101.c : line 664 : TS_len_u8=Make_Radian_Master_req(txbuffer, 16 , 123456 );
```

## Compiling

```
gcc radian_trx.c -o radian_trx -lwiringPi -lpthread -Wall
```

## Troubleshooting




## Origin and license

This code is fetched from http://www.lamaisonsimon.fr/wiki/doku.php?id=maison2:compteur_d_eau:compteur_d_eau

The license is unknown, citing author (fred):

> I didn't put a license on this code maybe I should, I didn't know much about it in terms of licensing.
> this code was made by "looking" at the radian protocol which is said to be open source earlier in the page, I don't know if that helps?




