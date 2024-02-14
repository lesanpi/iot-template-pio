#ifndef CONFIGURATION_USE_CASE_H
#define CONFIGURATION_USE_CASE_H
#include "Development.h"
#include "GPSManager.h"
#include "MemoryManager.h"
#include "BLEManager.h"
#include "InputManager.h"
#include "OutputManager.h"

class ConfigurationUseCase : public BLECharacteristicCallbacks
{
public:
    ConfigurationUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager, InputManager *inputManager, OutputManager *outputManager);
    void begin();
    void loop();
    bool isConfigured();
    void onWrite(BLECharacteristic *pCharacteristic)
    {

        std::string id = pCharacteristic->getValue();
        log("🛜 Value received id: " + String(id.c_str()), "ConfigurationUseCase.onWrite()");
        memoryManager->writeVehicleID(String(id.c_str()));

        log("🚗 Restarted distanceTraveled to 0...", "ConfigurationUseCase.onWrite()");
        gpsManager->restart();
        bleManager->updateKilometers(0);

        /// Restart to factory configuration
        if (String(id.c_str()).isEmpty())
        {
            this->outputManager->setState(DeviceState::NotConfigured);
            this->memoryManager->writeDistance(0);
        }
        else
        {
            this->outputManager->setState(DeviceState::Running);
        }
    }

private:
    MemoryManager *memoryManager;
    GPSManager *gpsManager;
    BLEManager *bleManager;
    InputManager *inputManager;
    OutputManager *outputManager;

    bool configured = false;
};

#endif