#include "ScannerUseCase.h"

ScannerUseCase::ScannerUseCase(
    ELM327Manager *elm327Manager,
    ELM327Manager *elm327BleManager,
    BLEManager *bleManager,
    WiFiScannerManager *wifiScannerManager,
    BTScannerManager *bleScannerManager)
{
    this->elm327Manager = elm327Manager;
    this->wifiScannerManager = wifiScannerManager;
    this->elm327BleManager = elm327BleManager;
    this->bleScannerManager = bleScannerManager;
    this->bleManager = bleManager;
}

void ScannerUseCase::begin()
{
}

void ScannerUseCase::loop()
{
    unsigned long now = millis();

    if (!bleScannerManager->isConnected())
        wifiScannerManager->loop();
    bool canConnectObd2 = now - lastConnectionObd2 > 1000 * 60 * 5;
    if (wifiScannerManager->isConnected() && !elm327Manager->isConnected() && canConnectObd2)
    {
        elm327Manager->begin();
        lastConnectionObd2 = now;
    }
    elm327Manager->loop();

    if (elm327Manager->hasDataAvailable())
    {
        String data =
            elm327Manager->getJSON();
        if (now - lastUpdateTime > 1000 * 60)
        {
            lastUpdateTime = now;
            bleManager->updateScannerData(data);
        }
        elm327Manager->disconnected();
        wifiScannerManager->disconnected();
        // if (data.compareTo(currentScannerData))
        // {
        //     log("✅ Has Data updated: " + data, "ScannerUseCase.loop()");
        //     currentScannerData = data;
        //     bleManager->updateScannerData(data);
        // }
        // else
        // {
        //     // log("❌ Data no updated", "ScannerUseCase.loop()");
        // }

        /// Optimize memory
        data.clear();
    }

    // /// BLE Scanner
    // // if (!wifiScannerManager->isConnected())
    // bleScannerManager->loop();
    // if (bleScannerManager->isConnected() && !elm327BleManager->isConnected())
    // {
    //     elm327BleManager->begin();
    // }
    // elm327BleManager->loop();

    // if (elm327BleManager->hasDataAvailable())
    // {
    //     String data =
    //         elm327BleManager->getJSON();
    //     bleManager->updateScannerData(data);
    //     if (now - lastUpdateTime > 1000 * 60)
    //     {
    //         lastUpdateTime = now;
    //         bleManager->updateScannerData(data);
    //     }

    //     // if (data.compareTo(currentScannerData))
    //     // {
    //     //     log("✅ Has Data updated: " + data, "ScannerUseCase.loop()");
    //     //     currentScannerData = data;
    //     //     bleManager->updateScannerData(data);
    //     // }
    //     // else
    //     // {
    //     //     // log("❌ Data no updated", "ScannerUseCase.loop()");
    //     // }

    //     /// Optimize memory
    //     data.clear();
    // }
}