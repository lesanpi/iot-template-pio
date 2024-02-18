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

    /// Memory distance traveled
    double currentTotalDistanceTraveled = memoryManager->getTotalDistanceTraveled();
    // log("🚗🔄 Distance traveled in progress... " + String(currentTotalDistanceTraveled) + " km", "MileageMeterUseCase.loop()");

    if (isConfigured)
        gpsManager->loop();
    if (gpsManager->isUpdated() && isConfigured)
    {
        /// GPS Distance traveled counter
        double distanceTraveledUpdated = gpsManager->getDistanceTraveled();
        log("🚗 Distance traveled detected... " + String(distanceTraveledUpdated, 6) + " km", "MileageMeterUseCase.loop()");

        double totalDistanceTraveledUpdated = currentTotalDistanceTraveled + distanceTraveledUpdated;
        log("🛣️ Total distance traveled... " + String(totalDistanceTraveledUpdated, 6) + " km", "MileageMeterUseCase.loop()");

        /// Restart GPS Manager counter
        gpsManager->restartDistanceTraveled();
        /// Update memory distance distanceTraveled
        memoryManager->writeDistance(totalDistanceTraveledUpdated);
        /// Send data via BLE
        bleManager->updateKilometers(totalDistanceTraveledUpdated);
    }
}