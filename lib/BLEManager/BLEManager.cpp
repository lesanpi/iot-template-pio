#include "BLEManager.h"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        pServer->startAdvertising(); // restart advertising
    };

    void onDisconnect(BLEServer *pServer)
    {
        pServer->startAdvertising(); // restart advertising
    }
};

BLEManager::BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *characteristicUUID)
{
    this->serviceUUID = serviceUUID;
    this->characteristicUUID = characteristicUUID;
    // Initialize BLE
    BLEDevice::init(deviceName);

    // Create server and service
    pServer = BLEDevice::createServer();
    pService = pServer->createService(this->serviceUUID);

    // Create and add characteristic
    pService->createCharacteristic(this->characteristicUUID, BLECharacteristic::PROPERTY_READ);
}

void BLEManager::begin()
{
    // Set server callbacks
    pServer->setCallbacks(new MyServerCallbacks());

    // Start service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(this->serviceUUID);
    // ... (other advertising settings)
    BLEDevice::startAdvertising();
}

void BLEManager::updateKilometers(int kilometers)
{
    characteristic->setValue(kilometers);
    characteristic->notify();
}