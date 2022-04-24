#include "everblu_meters.h"

void setup()
{
  Serial.begin(115200);

  pinMode(GDO2, INPUT);
  pinMode(GDO0, INPUT);

  cc1101_init();

  struct tmeter_data meter_data;
  meter_data = get_meter_data();

  Serial.print("Liters : ");
  Serial.println(meter_data.liters);
}

void loop()
{
  
}
