#include "ConfigurationUseCase.h"
#include "Development.h"

ConfigurationUseCase::ConfigurationUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager)
{
    this->memoryManager = memoryManager;
    this->gpsManager = gpsManager;
    this->bleManager = bleManager;
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