#include <Arduino.h>
#include "Development.h"
#include "GPSManager.h"
#include "BLEManager.h"
#include "MemoryManager.h"
#include "InputManager.h"
#include "OutputManager.h"
#include "ConfigurationUseCase.h"
#include "MileageMeterUseCase.h"
#include "WifiScannerManager.h"
#include "ELM327Manager.h"

/// TIMES
#define RESET_TIME_MAX 5000

/// PINOUT
#define RESET_BUTTON_PIN 34
#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27

/// BLE UUIDS
/// @brief  Characteristic UUID
const char *KILOMETERS_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
/// brief   Vehicle UUID
const char *VEHICLE_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a9";
/// brief   Geolocation UUID
const char *GEOLOCATION_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26b9";
/// @brief  Service UUID
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

/// Managers
BLEManager *bleManager;
GPSManager *gpsManager;
MemoryManager *memoryManager;
InputManager *inputManager;
OutputManager *outputManager;
WiFiScannerManager *wifiScannerManager;
ELM327Manager *elm327Manager;

/// Use cases
MileageMeterUseCase *mileageMeterUseCase;
ConfigurationUseCase *configurationUseCase;

/// Client
WiFiClient client;

int kilometers = 0;

void setup()
{
  Serial.begin(115200);
  // Serial.begin(9600);
  while (!Serial.availableForWrite())
    ;
  delay(2000);

  log("🤖 Starting setup...", "SETUP");

  // ...

  /// Managers
  inputManager = new InputManager(RESET_BUTTON_PIN, RESET_TIME_MAX);
  bleManager = new BLEManager("DEMO Lesanpi", SERVICE_UUID, KILOMETERS_CHARACTERISTIC_UUID, VEHICLE_CHARACTERISTIC_UUID, GEOLOCATION_CHARACTERISTIC_UUID);
  gpsManager = new GPSManager(32, 33, false);
  // gpsManager = new GPSManager(32, 33, true);
  memoryManager = new MemoryManager(1024);
  outputManager = new OutputManager(RED_PIN, GREEN_PIN, BLUE_PIN);
  /// Use cases
  configurationUseCase = new ConfigurationUseCase(memoryManager, gpsManager, bleManager, inputManager, outputManager);
  mileageMeterUseCase = new MileageMeterUseCase(memoryManager, gpsManager, bleManager, inputManager, outputManager);
  wifiScannerManager = new WiFiScannerManager("WiFi_OBDII", -55);
  elm327Manager = new ELM327Manager(client);
  /// Begin managers
  inputManager->setup();
  memoryManager->begin();
  bleManager->begin();
  gpsManager->begin();
  outputManager->setup();
  wifiScannerManager->begin();

  WiFi.onEvent([](WiFiEvent_t event)
               {
                log("Event received " + String(event), "WiFi.onEvent");
if (event == WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
  wifiScannerManager->disconnected();
  elm327Manager->disconnected();
        } }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  /// Begin use cases
  configurationUseCase->begin();
}

void loop()
{

  configurationUseCase->loop();
  mileageMeterUseCase->loop();
  wifiScannerManager->loop();
  if (wifiScannerManager->isConnected() && !elm327Manager->isConnected())
  {
    elm327Manager->begin();
  }
  elm327Manager->loop();

  // kilometers++;
  // log("🔄 Kilometers changed... " + String(kilometers) + " km", "LOOP");
  // bleManager->updateKilometers(kilometers);
  // gpsManager->loop();
  // delay(5000);

  // ...
}
