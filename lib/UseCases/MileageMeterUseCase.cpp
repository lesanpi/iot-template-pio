#include "MileageMeterUseCase.h"

MileageMeterUseCase::MileageMeterUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager, InputManager *inputManager, OutputManager *outputManager)
{
    this->memoryManager = memoryManager;
    this->gpsManager = gpsManager;
    this->bleManager = bleManager;
    this->inputManager = inputManager;
    this->outputManager = outputManager;
}

void MileageMeterUseCase::loop()
{
    bool isConfigured = memoryManager->isConfigured();
    if (isConfigured)
        gpsManager->loop();
    if (gpsManager->isUpdated() && isConfigured)
    {
        /// GPS Distance traveled counter
        double distanceTraveledUpdated = gpsManager->getDistanceTraveled();
        log("🚗 Distance traveled... " + String(distanceTraveledUpdated) + " km", "MileageMeterUseCase.loop()");

        /// Memory distance traveled
        double currentTotalDistanceTraveled = memoryManager->getTotalDistanceTraveled();
        double totalDistanceTraveledUpdated = currentTotalDistanceTraveled + distanceTraveledUpdated;
        log("🛣️ Total distance traveled... " + String(totalDistanceTraveledUpdated) + " km", "MileageMeterUseCase.loop()");

        /// Restart GPS Manager counter
        gpsManager->restartDistanceTraveled();
        /// Update memory distance distanceTraveled
        memoryManager->writeDistance(totalDistanceTraveledUpdated);
        /// Send data via BLE
        bleManager->updateKilometers(totalDistanceTraveledUpdated);
    }
}