#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <Arduino.h>

class MemoryManager
{
public:
    MemoryManager(size_t size);
    void begin();
    void writeDistance(double distance);
    double readDistance();
    void writeVehicleID(String vehicleID);
    String readVehicleID();

    String getVehicleId();
    double getTotalDistanceTraveled();
    bool isConfigured()
    {
        String vehicle = getVehicleId();
        return !vehicle.isEmpty();
    }

private:
    const int EEPROM_ADDR_DISTANCE = 0;
    const int EEPROM_ADDR_VEHICLE_ID = 8;
    String vehicleId = "";
    double distanceTraveled = 0;
    size_t size;

    int getAddrOfVehicle()
    {
        // return sizeof(this->distanceTraveled);
        return EEPROM_ADDR_VEHICLE_ID;
    }
};

#endif