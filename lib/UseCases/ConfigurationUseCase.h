#ifndef CONFIGURATION_USE_CASE_H
#define CONFIGURATION_USE_CASE_H
#include "Development.h"
#include "GPSManager.h"
#include "MemoryManager.h"
#include "BLEManager.h"
#include "InputManager.h"

class ConfigurationUseCase : public BLECharacteristicCallbacks
{
public:
    ConfigurationUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager, InputManager *inputManager);
    void begin();
    void loop();
    bool isConfigured();
    void onWrite(BLECharacteristic *pCharacteristic)
    {

        std::string id = pCharacteristic->getValue();
        log("🛜 Value received id: " + String(id.c_str()), "ConfigurationUseCase.onWrite()");
        memoryManager->writeVehicleID(String(id.c_str()));

        log("🚗 Restarted distanceTraveled to 0...", "ConfigurationUseCase.onWrite()");
        gpsManager->restartDistanceTraveled();
        bleManager->updateKilometers(0);
    }

private:
    MemoryManager *memoryManager;
    GPSManager *gpsManager;
    BLEManager *bleManager;
    InputManager *inputManager;

    bool configured = false;
};

#endif