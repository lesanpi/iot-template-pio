#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>
#include "Development.h"

class BLEManager
{
public:
    BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *vehicleUUID);
    void begin();
    void updateKilometers(int kilometers);
    void updateVehicleId(String vehicleId);
    void setVehicleCharacteristicCallback(BLECharacteristicCallbacks *pCallbacks);

private:
    const char *serviceUUID;
    const char *kilometersCharacteristicUUID;
    const char *vehicleCharacteristicUUID;
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *kilometersCharacteristic;
    BLECharacteristic *vehicleCharacteristic;
};

#endif