#include <Arduino.h>
#include "Development.h"
#include "GPSManager.h"
#include "BLEManager.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "ConfigurationUseCase.h"

/// TIMES
#define RESET_TIME_MAX 5000

/// PINOUT
#define RESET_BUTTON_PIN 34

/// BLE UUIDS
/// @brief  Characteristic UUID
const char *KILOMETERS_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
/// brief   Vehicle UUID
const char *VEHICLE_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a9";
/// @brief  Service UUID
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

/// Managers
BLEManager *bleManager;
GPSManager *gpsManager;
MemoryManager *memoryManager;
InputManager *inputManager;
/// Use cases
ConfigurationUseCase *configurationUseCase;

int kilometers = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial.availableForWrite())
    ;
  delay(2000);

  log("🤖 Starting setup...", "SETUP");

  // ...

  /// Managers
  inputManager = new InputManager(RESET_BUTTON_PIN, RESET_TIME_MAX);
  bleManager = new BLEManager("DEMO Lesanpi", SERVICE_UUID, KILOMETERS_CHARACTERISTIC_UUID, VEHICLE_CHARACTERISTIC_UUID);
  gpsManager = new GPSManager(4, 3, true);
  memoryManager = new MemoryManager(1024);
  /// Use cases
  configurationUseCase = new ConfigurationUseCase(memoryManager, gpsManager, bleManager, inputManager);

  /// Begin managers
  inputManager->setup();
  memoryManager->begin();
  bleManager->begin();
  gpsManager->begin();

  /// Begin use cases
  configurationUseCase->begin();
}

void loop()
{
  // ...
  configurationUseCase->loop();

  // kilometers++;
  // log("🔄 Kilometers changed... " + String(kilometers) + " km", "LOOP");
  // bleManager->updateKilometers(kilometers);
  // gpsManager->loop();
  // delay(5000);

  // ...
}