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
};
#endif