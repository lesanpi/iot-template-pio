#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class BLEManager
{
public:
    BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *characteristicUUID);
    void begin();
    void updateKilometers(int kilometers);

private:
    const char *serviceUUID;
    const char *characteristicUUID;
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *characteristic;
};

#endif