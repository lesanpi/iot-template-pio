#ifndef CONFIGURATION_USE_CASE_H
#define CONFIGURATION_USE_CASE_H
#include "GPSManager.h"
#include "MemoryManager.h"
#include "BLEManager.h"
#include "Development.h"

class ConfigurationUseCase : public BLECharacteristicCallbacks
{
public:
    ConfigurationUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager);
    void begin();
    bool isConfigured();
    void onWrite(BLECharacteristic *pCharacteristic)
    {

        std::string id = pCharacteristic->getValue();
        log("🛜 Value received id: " + String(id.c_str()), "ConfigurationUseCase.onWrite()");
        memoryManager->writeVehicleID(String(id.c_str()));

        log("🚗 Restarted distanceTraveled to 0...", "ConfigurationUseCase.onWrite()");
        gpsManager->restartDistanceTraveled();
    }

private:
    MemoryManager *memoryManager;
    GPSManager *gpsManager;
    BLEManager *bleManager;

    bool configured = false;
};

#endif