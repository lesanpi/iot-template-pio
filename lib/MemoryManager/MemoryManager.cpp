#include "MemoryManager.h"
#include "Development.h"
#include <EEPROM.h>

MemoryManager::MemoryManager(size_t size)
{
    this->size = size;
}

void MemoryManager::begin()
{
    log("Starting Memory Manager...", "MemoryManager.begin()");
    EEPROM.begin(this->size);
    double distanceRead = this->readDistance();
    String vehicleIdRead = this->readVehicleID();

    log("Vehicle id saved: " + vehicleIdRead, "MemoryManager.begin()");
    log("Distance saved: " + String(distanceRead), "MemoryManager.begin()");
    if (vehicleIdRead.isEmpty())
    {
        log("No vehicle configured", "MemoryManager.begin()");
    }

    this->vehicleId = vehicleIdRead;
    if (!isnan(distanceRead))
    {
        this->distanceTraveled = distanceRead;
    }
}

void MemoryManager::writeDistance(double distance)
{
    // Write the distance to the memory EEPROM
    EEPROM.writeDouble(EEPROM_ADDR_DISTANCE, distance);
    this->distanceTraveled = distance;
}

double MemoryManager::readDistance()
{
    /// Read the distance from the memory EEPROM
    return EEPROM.readDouble(EEPROM_ADDR_DISTANCE);
}

void MemoryManager::writeVehicleID(String vehicleID)
{
    /// Write the vehicle id to the memory EEPROM
    EEPROM.writeString(EEPROM_ADDR_VEHICLE_ID, vehicleID);
    this->vehicleId = vehicleID;
}

String MemoryManager::readVehicleID()
{
    /// Read the vehicle id from the memory EEPROM
    return EEPROM.readString(EEPROM_ADDR_VEHICLE_ID);
}
