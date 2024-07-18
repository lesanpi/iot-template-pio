#include "BLEScannerManager.h"

BTScannerManager::BTScannerManager(const char *bleName, BluetoothSerial &bleSerial, int minRSSI) : bleSerial(bleSerial)
{

    this->bleName = bleName;
    this->minRSSI = minRSSI;
}

void BTScannerManager::scanDevices()
{

    if (bleSerial.connected())
    {
        log("Ya esta conectado a un BLE Device.", "BLEScannerManager.scanDevices()");
        return;
    }

    log("🔍 Running scan", "BLEScannerManager.loop()");

    // hasDeviceFound = false;
    BTScanResults *results = bleSerial.getScanResults();
    for (size_t i = 0; i < results->getCount(); i++)
    {
        BTAdvertisedDevice *device = results->getDevice(i);
        // log("🛜 Dispositivo scaneado:  " + String(device->getName().c_str()) + " " + device->getAddress().toString().c_str(), "BLEScannerManager.scanDevices()");
        if (strcmp(device->getName().c_str(), "OBDII") == 0)
        {
            log("✅ Dispositivo encontrado!", "BLEScannerManager.scanDevices()");
            bleSerial.discoverAsyncStop();
            setDevice(device);
            return;
        }
    }

    bleSerial.discoverClear();
    bleSerial.discoverAsyncStop();
    if (bleSerial.discoverAsync(callback))
    {
        log("Findings will be reported in \"btAdvertisedDeviceFound\"", "BLEScannerManager.scanDevices()");

        // Serial.print("Stopping discoverAsync... ");
        // bleSerial.discoverAsyncStop();
        // Serial.println("stopped");
    }
    else
    {
        log("Error on discoverAsync f.e. not working after a \"connect\"", "BLEScannerManager.scanDevices()");
    }
}
