#include "ConfigurationUseCase.h"
#include "Development.h"
#include "InputManager.h"

ConfigurationUseCase::ConfigurationUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager, InputManager *inputManager, OutputManager *outputManager, ELM327Manager *elm327Manager, ELM327Manager *elm327BtManager)
{
    this->memoryManager = memoryManager;
    this->gpsManager = gpsManager;
    this->bleManager = bleManager;
    this->inputManager = inputManager;
    this->outputManager = outputManager;
    this->elm327Manager = elm327Manager;
    this->elm327BtManager = elm327BtManager;
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

    if (alreadyConfigured)
    {
        this->outputManager->setState(DeviceState::Running);
    }
    else
    {
        this->outputManager->setState(DeviceState::NotConfigured);
    }

    /// Set bluetooth callback
    bleManager->setVehicleCharacteristicCallback(this);
    log("Vehicle characteristic callback setted", "ConfigurationUseCase.begin()");
    elm327Manager->setVehicleId(memoryManager->getVehicleId());
    elm327BtManager->setVehicleId(memoryManager->getVehicleId());
    bleManager->updateVehicleId(memoryManager->getVehicleId());
    bleManager->updateKilometers(memoryManager->getTotalDistanceTraveled());
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
        gpsManager->restart();
        /// Update characteristic data
        bleManager->updateKilometers(0);
        bleManager->updateVehicleId("");
        /// Restart reset button
        inputManager->restarted();
        /// Change output state
        this->outputManager->setState(DeviceState::NotConfigured);
    }
}