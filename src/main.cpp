#include "GPSManager.h"
#include "BLEManager.h"
#include "MemoryManager.h"
#include <Arduino.h>
#include "Development.h"

/// @brief  Characteristic UUID
const char *CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
/// @brief  Service UUID
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

BLEManager *bleManager;
GPSManager *gpsManager;
MemoryManager *memoryManager;

int kilometers = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial.availableForWrite())
    ;
  log("🤖 Starting setup...", "SETUP");

  // ...

  bleManager = new BLEManager("DEMO Lesanpi", SERVICE_UUID, CHARACTERISTIC_UUID);
  gpsManager = new GPSManager(4, 3, true);
  memoryManager = new MemoryManager(1024);
  memoryManager->begin();
  bleManager->begin();
  gpsManager->begin();
}

void loop()
{
  // ...

  kilometers++;
  log("🔄 Kilometers changed... " + String(kilometers) + " km", "LOOP");
  bleManager->updateKilometers(kilometers);
  gpsManager->loop();
  delay(2000);

  // ...
}