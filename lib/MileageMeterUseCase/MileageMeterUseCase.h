#ifndef MILEAGE_METER_USE_CASE_H
#define MILEAGE_METER_USE_CASE_H
#include "Development.h"
#include "GPSManager.h"
#include "MemoryManager.h"
#include "BLEManager.h"
#include "InputManager.h"
#include "OutputManager.h"

class MileageMeterUseCase
{
public:
    MileageMeterUseCase(MemoryManager *memoryManager, GPSManager *gpsManager, BLEManager *bleManager, InputManager *inputManager, OutputManager *outputManager);
    void loop();

private:
    MemoryManager *memoryManager;
    GPSManager *gpsManager;
    BLEManager *bleManager;
    InputManager *inputManager;
    OutputManager *outputManager;
    /// @brief Last location known
    GeolocationData lastLocation;

    bool lastGpsConnectionState = false;

    /// @brief Determinates if the geolocation was updated
    /// @return true if the location was updated
    bool geolocationIsUpdated()
    {
        if (gpsManager->isInitialized())
        {
            return false;
        }

        GeolocationData geolocation = gpsManager->geolocation();
        return geolocation.latitude != lastLocation.latitude || geolocation.longitude != lastLocation.longitude;
    }

    void setGeolocation(GeolocationData data)
    {
        lastLocation.latitude = data.latitude;
        lastLocation.longitude = data.longitude;
    }
};
#endif