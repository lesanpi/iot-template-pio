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
        bleSerial.discoverAsyncStop();
        bleSerial.discoverClear();
        log("Ya esta conectado a un BLE Device.", "BLEScannerManager.scanDevices()");
        return;
    }

    log("🔍 Running scan", "BLEScannerManager.loop()");
    hasDeviceFound = false;
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
