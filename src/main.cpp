#include "BLEManager.h"
#include <Arduino.h>

/// @brief  Characteristic UUID
const char *CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
/// @brief  Service UUID
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

BLEManager *bleManager;
int kilometers = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial.availableForWrite())
    ;
  // ...

  bleManager = new BLEManager("DEMO Lesanpi", SERVICE_UUID, CHARACTERISTIC_UUID);
  bleManager->begin();
}

void loop()
{
  // ...

  kilometers++;
  bleManager->updateKilometers(kilometers);
  delay(2000);

  // ...
}