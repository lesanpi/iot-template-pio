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

BLEManager::BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *hasGpsConnectionUUID, const char *vehicleUUID, const char *geolocationUUID, const char *scannerDataUUID)
{
    this->serviceUUID = serviceUUID;
    this->kilometersCharacteristicUUID = kilometersUUID;
    this->hasGpsConnectionCharacteristicUUID = hasGpsConnectionUUID;
    this->vehicleCharacteristicUUID = vehicleUUID;
    this->geolocationCharacteristicUUID = geolocationUUID;
    this->scannerDataCharacteristicUUID = scannerDataUUID;
    // Initialize BLE
    BLEDevice::init(deviceName);

    // Create server and service
    pServer = BLEDevice::createServer();
    pService = pServer->createService(this->serviceUUID);

    // Create and add characteristic
    kilometersCharacteristic = pService->createCharacteristic(this->kilometersCharacteristicUUID, BLECharacteristic::PROPERTY_READ);
    hasGPSConnectionCharacteristic = pService->createCharacteristic(this->hasGpsConnectionCharacteristicUUID, BLECharacteristic::PROPERTY_READ);
    geolocationCharacteristic = pService->createCharacteristic(this->geolocationCharacteristicUUID, BLECharacteristic::PROPERTY_READ);
    vehicleCharacteristic = pService->createCharacteristic(this->vehicleCharacteristicUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    scannerDataCharacteristic = pService->createCharacteristic(this->scannerDataCharacteristicUUID, BLECharacteristic::PROPERTY_READ);
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
    log("kilometers: " + String(kilometers), "BLEManager.updateKilometers");
    kilometersCharacteristic->setValue(kilometers);
    kilometersCharacteristic->notify();
}
void BLEManager::updateHasGpsConnection(int connected)
{
    log("Has gps connection: " + String(connected), "BLEManager.updateHasGpsConnection");
    hasGPSConnectionCharacteristic->setValue(connected);
    hasGPSConnectionCharacteristic->notify();
}

void BLEManager::updateVehicleId(String id)
{
    vehicleCharacteristic->setValue(id.c_str());
    vehicleCharacteristic->notify();
}

void BLEManager::updateScannerData(String data)
{

    // log("OBD2 Scanner data updated", "BLEManager.updateScannerData");
    scannerDataCharacteristic->setValue(data.c_str());
    scannerDataCharacteristic->notify();
}
void BLEManager::updateGeolocation(const GeolocationData &data)
{
    // Create a JSON object
    JsonDocument doc;

    JsonObject root = doc.to<JsonObject>();
    root["latitude"] = data.latitude;
    root["longitude"] = data.longitude;

    // Convert JSON object to string
    char jsonBuffer[256]; // Adjust size based on data complexity
    serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

    // Set the value of the geolocation characteristic
    geolocationCharacteristic->setValue(jsonBuffer);

    log("Geolocation updated: " + String(data.latitude) + "," + String(data.longitude), "BLEManager.updateGeolocation");
}