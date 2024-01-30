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
        log("🚨 Restarting to factory configuration", "ConfigurationUseCase.mustRestartDevice()");
        /// Clean memory
        memoryManager->writeVehicleID("");
        memoryManager->writeDistance(0);
        /// Restart distance traveled counter
        gpsManager->restartDistanceTraveled();
        /// Update characteristic data
        bleManager->updateKilometers(0);
        /// Restart reset button
        inputManager->restarted();
    }
}