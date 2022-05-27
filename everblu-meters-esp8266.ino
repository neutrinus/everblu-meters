#include "everblu_meters.h"

// Project source : 
// http://www.lamaisonsimon.fr/wiki/doku.php?id=maison2:compteur_d_eau:compteur_d_eau

// Require EspMQTTClient library (by Patrick Lapointe) version 1.13.3
// Install from Arduino library manager (and its dependancies)
// https://github.com/plapointe6/EspMQTTClient/releases/tag/1.13.3
#include "EspMQTTClient.h"

// Edit "everblu_meters.h" file then change the define at the end of the file


EspMQTTClient mqtt(
  "MyESSID",            // Your Wifi SSID
  "MyWiFiKey",          // Your WiFi key
  "mqtt.server.com",    // MQTT Broker server ip
  "MQTTUsername",       // Can be omitted if not needed
  "MQTTPassword",       // Can be omitted if not needed
  "EverblueCyble",      // Client name that uniquely identify your device
  1883                  // MQTT Broker server port
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

  digitalWrite(LED_BUILTIN, LOW); // turned on

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
  Serial.println("\n");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turned off

  //mqtt.enableDebuggingMessages(true);

  /*
  // Use this piece of code to find the right frequency.
  for (float i = 433.76f; i < 433.890f; i += 0.0005f) {
    Serial.printf("Test frequency : %f\n", i);
    cc1101_init(i);

    struct tmeter_data meter_data;
    meter_data = get_meter_data();

    if (meter_data.reads_counter != 0 || meter_data.liters != 0) {
      Serial.printf("\n------------------------------\nGot frequency : %f\n------------------------------\n", i);

      Serial.printf("Liters : %d\nBattery (in months) : %d\nCounter : %d\n\n", meter_data.liters, meter_data.battery_left, meter_data.reads_counter);

      digitalWrite(LED_BUILTIN, LOW); // turned on

      while (42);
    }
  }
  */


  
  cc1101_init(FREQUENCY);

  /*
  // Use this piece of code to test
  struct tmeter_data meter_data;
  meter_data = get_meter_data();
  Serial.printf("\nLiters : %d\nBattery (in months) : %d\nCounter : %d\nTime start : %d\nTime end : %d\n\n", meter_data.liters, meter_data.battery_left, meter_data.reads_counter, meter_data.time_start, meter_data.time_end);
  while (42); 
  */
}

void loop()
{
  mqtt.loop();
}
