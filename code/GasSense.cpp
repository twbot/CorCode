#include <stdint.h>
#include "GasSense.h"

uint32_t GasSense::gasSensorRead() {
  int sensorRead = analogRead(gasSensor);
  Serial.println("Gas Sensor =");
  Serial.print(sensorRead);
  return sensorRead;
}