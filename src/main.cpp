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

String deviceName = "DEMO Car 2";

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
  /// Callback on device found
  BTAdvertisedDeviceCb callback = [](BTAdvertisedDevice *device)
  {
    log("🛜 Dispositivo scaneado:  " + String(device->getName().c_str()) + " " + device->getAddress().toString().c_str(), "BLEScannerManager.scanDevices()");
    if (strcmp(device->getName().c_str(), "OBDII") == 0)
    {
      log("✅ Dispositivo encontrado!", "BLEScannerManager.scanDevices()");
      btScannerManager->setDevice(device);
    }
  };
  /// Set Callback
  btScannerManager->setCallback(callback);
  elm327BLEManager = new ELM327Manager(client, serialBT, false, 10000, false, ELM_Manager_Type::ELM_BLE);
  elm327Manager = new ELM327Manager(client, serialBT, false, 10000, false, ELM_Manager_Type::ELM_WIFI);

  /// Use cases
  configurationUseCase = new ConfigurationUseCase(memoryManager, gpsManager, bleManager, inputManager, outputManager, elm327Manager);
  mileageMeterUseCase = new MileageMeterUseCase(memoryManager, gpsManager, bleManager, inputManager, outputManager);
  scannerUseCase = new ScannerUseCase(elm327Manager, elm327BLEManager, bleManager, wifiScannerManager, btScannerManager);

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
                 elm327Manager->disconnected();
                 // if (event == WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
                 //         }
               },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  // uint8_t address[6] = {0x1c, 0xA1, 0x35, 0x69, 0x8D, 0xC5};
  // serialBT.connect(address);
}

void loop()
{

  // configurationUseCase->loop();
  // mileageMeterUseCase->loop();
  scannerUseCase->loop();
  // btScannerManager->loop();

  // wifiScannerManager->loop();
  // if (wifiScannerManager->isConnected() && !elm327Manager->isConnected())
  // {
  //   elm327Manager->begin();
  // }
  // elm327Manager->loop();

  // kilometers++;
  // log("🔄 Kilometers changed... " + String(kilometers) + " km", "LOOP");
  // bleManager->updateKilometers(kilometers);
  // gpsManager->loop();
  // delay(5000);

  // ...
}

// #include "BluetoothSerial.h"
// #include "ELMduino.h"

// BluetoothSerial SerialBT;
// #define ELM_PORT SerialBT
// #define DEBUG_PORT Serial
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// ELM327 myELM327;

// typedef enum
// {
//   ENG_RPM,
//   SPEED
// } obd_pid_states;
// obd_pid_states obd_state = ENG_RPM;

// float rpm = 0;
// float mph = 0;

// // 1c:a1:35:69:8d:c5
// uint8_t address[6] = {0x1c, 0xA1, 0x35, 0x69, 0x8D, 0xC5};

// void setup()
// {
//   DEBUG_PORT.begin(115200);
//   // SerialBT.setPin("1234");
//   ELM_PORT.begin("ArduHUD", true);
//   ELM_PORT.setPin("1234");
//   ELM_PORT.setTimeout(60000);
//   DEBUG_PORT.println("Starting Phase 1");

//   // if (!ELM_PORT.connect("OBDII"))
//   if (!ELM_PORT.connect(address))
//   {
//     DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
//     while (1)
//       ;
//   }

//   if (!myELM327.begin(ELM_PORT, false, 2000))
//   {
//     DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 2");
//     while (1)
//       ;
//   }

//   DEBUG_PORT.println("Connected to ELM327");
// }

// void loop()
// {
//   switch (obd_state)
//   {
//   case ENG_RPM:
//   {
//     rpm = myELM327.rpm();

//     if (myELM327.nb_rx_state == ELM_SUCCESS)
//     {
//       DEBUG_PORT.print("rpm: ");
//       DEBUG_PORT.println(rpm);
//       obd_state = SPEED;
//     }
//     else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
//     {
//       myELM327.printError();
//       obd_state = SPEED;
//     }

//     break;
//   }

//   case SPEED:
//   {
//     mph = myELM327.mph();

//     if (myELM327.nb_rx_state == ELM_SUCCESS)
//     {
//       DEBUG_PORT.print("mph: ");
//       DEBUG_PORT.println(mph);
//       obd_state = ENG_RPM;
//     }
//     else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
//     {
//       myELM327.printError();
//       obd_state = ENG_RPM;
//     }

//     break;
//   }
//   }
// }