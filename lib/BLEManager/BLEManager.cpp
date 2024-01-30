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

BLEManager::BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *vehicleUUID)
{
    this->serviceUUID = serviceUUID;
    this->kilometersCharacteristicUUID = kilometersUUID;
    this->vehicleCharacteristicUUID = vehicleUUID;
    // Initialize BLE
    BLEDevice::init(deviceName);

    // Create server and service
    pServer = BLEDevice::createServer();
    pService = pServer->createService(this->serviceUUID);

    // Create and add characteristic
    kilometersCharacteristic = pService->createCharacteristic(this->kilometersCharacteristicUUID, BLECharacteristic::PROPERTY_READ);
    vehicleCharacteristic = pService->createCharacteristic(this->vehicleCharacteristicUUID, BLECharacteristic::PROPERTY_READ |
                                                                                                BLECharacteristic::PROPERTY_WRITE);
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
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void BLEManager::setVehicleCharacteristicCallback(BLECharacteristicCallbacks *pCallbacks)
{
    vehicleCharacteristic->setCallbacks(pCallbacks);
}

void BLEManager::updateKilometers(int kilometers)
{
    kilometersCharacteristic->setValue(kilometers);
    kilometersCharacteristic->notify();
}