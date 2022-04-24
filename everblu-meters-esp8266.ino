#include "everblu_meters.h"

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
  // Call back this function in 12 hours (in miliseconds)
  mqtt.executeDelayed(1000 * 60 * 60 * 12, onUpdateData);

  struct tmeter_data meter_data;
  meter_data = get_meter_data();

  Serial.printf("Liters : %d\nBattery (in months) : %d\nCounter : %d\n\n", meter_data.liters, meter_data.battery_left, meter_data.reads_counter);

  mqtt.publish("everblu/cyble/liters", String(meter_data.liters).c_str(), true);
  mqtt.publish("everblu/cyble/counter", String(meter_data.reads_counter).c_str(), true);
  mqtt.publish("everblu/cyble/battery", String(meter_data.battery_left).c_str(), true);
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
