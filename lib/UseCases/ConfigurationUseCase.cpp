#include "ConfigurationUseCase.h"
#include "Development.h"

ConfigurationUseCase::ConfigurationUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager, InputManager *inputManager)
{
    this->memoryManager = memoryManager;
    this->gpsManager = gpsManager;
    this->bleManager = bleManager;
    this->inputManager = inputManager;
}

bool ConfigurationUseCase::isConfigured()
{
    String vehicleSaved = memoryManager->getVehicleId();
    bool alreadyConfigured = !vehicleSaved.isEmpty();
    return alreadyConfigured;
}

void ConfigurationUseCase::begin()
{
    log("Configuration started", "ConfigurationUseCase.begin()");

    /// Verify if device is already configured
    bool alreadyConfigured = this->isConfigured();
    log("Device is already configured: " + String(alreadyConfigured), "ConfigurationUseCase.begin()");

    /// Set bluetooth callback
    bleManager->setVehicleCharacteristicCallback(this);
    log("Vehicle characteristic callback setted", "ConfigurationUseCase.begin()");
}

void ConfigurationUseCase::loop()
{
    inputManager->loop();
    if (inputManager->mustRestartDevice())
    {

        log("🚨 Restarting memory data to default", "ConfigurationUseCase.mustRestartDevice()");
        memoryManager->writeVehicleID("");
        memoryManager->writeDistance(0);

        log("🚨 Restarting gps distance traveled counter", "ConfigurationUseCase.mustRestartDevice()");
        gpsManager->restartDistanceTraveled();

        log("🚨 Updating BLE kilometers characteristic", "ConfigurationUseCase.mustRestartDevice()");
        bleManager->updateKilometers(0);

        log("🚨✅ Restart to default configuration finished", "ConfigurationUseCase.mustRestartDevice()");
        inputManager->restarted();
    }
}