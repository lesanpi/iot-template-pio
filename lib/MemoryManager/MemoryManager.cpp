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
    log("Distance saved: " + String(distanceRead, 3), "MemoryManager.begin()");
    if (vehicleIdRead.isEmpty())
    {
        log("No vehicle configured", "MemoryManager.begin()");
    }

    this->vehicleId = vehicleIdRead;
    if (!isnan(distanceRead))
    {
        this->distanceTraveled = distanceRead;
    }
    {
        this->distanceTraveled = 0;
        writeDistance(0);
    }
}

double MemoryManager::getTotalDistanceTraveled()
{
    return this->distanceTraveled;
}

void MemoryManager::writeDistance(double distance)
{
    // Write the distance to the memory EEPROM
    EEPROM.writeDouble(EEPROM_ADDR_DISTANCE, distance);
    EEPROM.commit();
    this->distanceTraveled = distance;
    double kilometers = readDistance();
    log("⚡️🚗 Kilometers saved " + String(kilometers, 3), "MemoryManager.writeDistance()");
}

double MemoryManager::readDistance()
{
    /// Read the distance from the memory EEPROM
    return EEPROM.readDouble(EEPROM_ADDR_DISTANCE);
}

void MemoryManager::writeVehicleID(String vehicleID)
{
    /// Write the vehicle id to the memory EEPROM
    EEPROM.writeString(getAddrOfVehicle(), vehicleID);
    EEPROM.commit();

    this->vehicleId = vehicleID;
    log("⚡️🚗 New vehicle id saved to storage... " + vehicleID, "MemoryManager.writeVehicleID()");

    /// Restart to initial state
    if (vehicleID.isEmpty() || this->vehicleId != vehicleID)
    {
        writeDistance(0);
        log("🔄 Restarting memory to initial state... " + vehicleID, "MemoryManager.writeVehicleID()");
    }
}

String MemoryManager::readVehicleID()
{
    /// Read the vehicle id from the memory EEPROM
    return EEPROM.readString(getAddrOfVehicle());
}

String MemoryManager::getVehicleId()
{
    return this->vehicleId;
}