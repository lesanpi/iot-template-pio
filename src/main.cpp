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
#include <ScannerUseCase.h>
#include <BLEScannerManager.h>

/// TIMES
#define RESET_TIME_MAX 5000

/// PINOUT
#define RESET_BUTTON_PIN 34
#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27

#define EVENT_BIT_1 (1 << 0)
#define EVENT_BIT_2 (1 << 1)

/// BLE UUIDS
/// @brief  Characteristic UUID
const char *KILOMETERS_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
/// brief   Vehicle UUID
const char *VEHICLE_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a9";
/// brief   Geolocation UUID
const char *GEOLOCATION_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26b9";
/// @brief OBD Scanner UUID
const char *SCANNER_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26b8";
/// @brief  Service UUID
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

/// Managers
BLEManager *bleManager;
GPSManager *gpsManager;
MemoryManager *memoryManager;
InputManager *inputManager;
OutputManager *outputManager;
WiFiScannerManager *wifiScannerManager;
BTScannerManager *btScannerManager;
ELM327Manager *elm327Manager;
ELM327Manager *elm327BLEManager;

// /// SSID List for OBDII
SSIDList ssidList = {
    .ssids = {"WiFi_OBDII", "WIFI-OBDII"},
    .numSsids = 2};

/// Use cases

/// Measure, save and publish the mileage by GPS
MileageMeterUseCase *mileageMeterUseCase;
/// Configuration and reset of the device and set the values according the state
ConfigurationUseCase *configurationUseCase;
/// Connect and read OBD2 Scanner data and update the data over BLE
ScannerUseCase *scannerUseCase;

// /// Client
WiFiClient client;
// /// BLE Client Serial
BluetoothSerial serialBT;

EventGroupHandle_t eventGroup;

String deviceName = "DEMO Lesanpi";

/// Callback on device found
BTAdvertisedDeviceCb callback = [](BTAdvertisedDevice *device)
{
  log("🛜 Dispositivo scaneado:  " + String(device->getName().c_str()) + " " + device->getAddress().toString().c_str(), "BLEScannerManager.callback()");
  if (strcmp(device->getName().c_str(), "OBDII") == 0)
  {
    log("✅ Dispositivo encontrado!", "BLEScannerManager.callback()");
  }
};

void setup()
{
  Serial.begin(115200);
  while (!Serial.availableForWrite())
    ;
  serialBT.begin(deviceName, true);
  serialBT.setPin("1234");
  serialBT.setTimeout(60000);
  log("🤖 Starting setup...", "SETUP");

  /// Managers
  inputManager = new InputManager(RESET_BUTTON_PIN, RESET_TIME_MAX);
  bleManager = new BLEManager(deviceName.c_str(), SERVICE_UUID, KILOMETERS_CHARACTERISTIC_UUID, VEHICLE_CHARACTERISTIC_UUID, GEOLOCATION_CHARACTERISTIC_UUID, SCANNER_CHARACTERISTIC_UUID);
  gpsManager = new GPSManager(32, 33, false);
  // // gpsManager = new GPSManager(32, 33, true);
  memoryManager = new MemoryManager(1024);
  outputManager = new OutputManager(RED_PIN, GREEN_PIN, BLUE_PIN);

  /// Options:
  ///  1. WiFi_OBDII
  ///  2. WIFI-OBDII
  wifiScannerManager = new WiFiScannerManager(ssidList, -55);
  /// Bluetooth scanner manager
  btScannerManager = new BTScannerManager("OBDII", serialBT, -60);
  /// Set Callback
  btScannerManager->setCallback(callback);
  elm327BLEManager = new ELM327Manager(client, serialBT, false, 10000, false, ELM_Manager_Type::ELM_BLE);
  elm327Manager = new ELM327Manager(client, serialBT, false, 10000, false, ELM_Manager_Type::ELM_WIFI);

  /// Use cases
  configurationUseCase = new ConfigurationUseCase(memoryManager,
                                                  gpsManager,
                                                  bleManager,
                                                  inputManager,
                                                  outputManager,
                                                  elm327Manager,
                                                  elm327BLEManager);
  mileageMeterUseCase = new MileageMeterUseCase(memoryManager, gpsManager, bleManager, inputManager, outputManager);
  scannerUseCase = new ScannerUseCase(
      elm327Manager,
      elm327BLEManager,
      bleManager,
      wifiScannerManager,
      btScannerManager);

  /// Begin managers
  inputManager->setup();
  memoryManager->begin();
  bleManager->begin();
  gpsManager->begin();
  outputManager->setup();
  wifiScannerManager->begin();
  configurationUseCase->begin();

  WiFi.onEvent([](WiFiEvent_t event)
               {
                 log("Event received " + String(event), "WiFi.onEvent");
                 wifiScannerManager->disconnected();
                 elm327Manager->disconnected(); },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

void loop()
{

  configurationUseCase->loop();
  mileageMeterUseCase->loop();
  scannerUseCase->loop();
}
