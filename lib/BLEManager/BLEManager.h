#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>
#include "Development.h"
#include <GPSManager.h>

class BLEManager
{
public:
    BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *hasGpsConnectionUUID, const char *vehicleUUID, const char *geolocationUUID, const char *scannerDataUUID);
    void begin();
    void updateKilometers(int kilometers);
    void updateHasGpsConnection(int connected);
    void updateVehicleId(String vehicleId);
    void updateScannerData(String data);
    void updateGeolocation(const GeolocationData &vehicleId);
    void setVehicleCharacteristicCallback(BLECharacteristicCallbacks *pCallbacks);

private:
    const char *serviceUUID;
    const char *kilometersCharacteristicUUID;
    const char *hasGpsConnectionCharacteristicUUID;
    const char *vehicleCharacteristicUUID;
    const char *geolocationCharacteristicUUID;
    const char *scannerDataCharacteristicUUID;
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *kilometersCharacteristic;
    BLECharacteristic *hasGPSConnectionCharacteristic;
    BLECharacteristic *vehicleCharacteristic;
    BLECharacteristic *geolocationCharacteristic;
    BLECharacteristic *scannerDataCharacteristic;
};

#endif