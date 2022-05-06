#include "everblu_meters.h"

// Project source : 
// http://www.lamaisonsimon.fr/wiki/doku.php?id=maison2:compteur_d_eau:compteur_d_eau

// Require EspMQTTClient library (by Patrick Lapointe) version 1.13.3
// Install from Arduino library manager (and its dependancies)
// https://github.com/plapointe6/EspMQTTClient/releases/tag/1.13.3
#include "EspMQTTClient.h"


EspMQTTClient mqtt(
  "WifiSSID",       // Your Wifi SSID
  "WifiPassword",   // Your WiFi key
  "192.168.1.100",  // MQTT Broker server ip
  "MQTTUsername",   // Can be omitted if not needed
  "MQTTPassword",   // Can be omitted if not needed
  "EverblueCyble",  // Client name that uniquely identify your device
  1883              // MQTT Broker server port
);

void onUpdateData()
{
  struct tmeter_data meter_data;
  meter_data = get_meter_data();

  if (meter_data.reads_counter == 0 && meter_data.liters == 0) {
    Serial.println("Unable to retrieve data from meter. Retry later...");
    // Call back this function in 1 hours (in miliseconds)
    mqtt.executeDelayed(1000 * 60 * 60 * 1, onUpdateData);
    return;
  }

  Serial.printf("Liters : %d\nBattery (in months) : %d\nCounter : %d\n\n", meter_data.liters, meter_data.battery_left, meter_data.reads_counter);

  mqtt.publish("everblu/cyble/liters", String(meter_data.liters, DEC), true);
  delay(50); // Do not remove
  mqtt.publish("everblu/cyble/counter", String(meter_data.reads_counter, DEC), true);
  delay(50); // Do not remove
  mqtt.publish("everblu/cyble/battery", String(meter_data.battery_left, DEC), true);

  // Call back this function in 24 hours (in miliseconds)
  mqtt.executeDelayed(1000 * 60 * 60 * 24, onUpdateData);
}

void onConnectionEstablished()
{
  Serial.println("Connected to MQTT Broker :)");

  onUpdateData();
}

void setup()
{
  Serial.begin(115200);

  cc1101_init();
}

void loop()
{
  mqtt.loop();
}
