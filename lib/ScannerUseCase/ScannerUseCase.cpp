#include "ScannerUseCase.h"

ScannerUseCase::ScannerUseCase(ELM327Manager *elm327Manager, ELM327Manager *elm327BleManager, BLEManager *bleManager, WiFiScannerManager *wifiScannerManager, BTScannerManager *bleScannerManager)
{
    this->elm327Manager = elm327Manager;
    this->elm327BleManager = elm327BleManager;
    this->wifiScannerManager = wifiScannerManager;
    this->bleScannerManager = bleScannerManager;
    this->bleManager = bleManager;
}

void ScannerUseCase::begin()
{
}

void ScannerUseCase::loop()
{
    wifiScannerManager->loop();
    if (wifiScannerManager->isConnected() && !elm327Manager->isConnected())
    {
        elm327Manager->begin();
    }
    elm327Manager->loop();
    if (elm327Manager->hasDataAvailable())
    {
        String data =
            elm327Manager->getJSON();

        if (data.compareTo(currentScannerData))
        {
            log("✅ Has Data updated: " + data, "ScannerUseCase.loop()");
            currentScannerData = data;
            bleManager->updateScannerData(data);
        }
        else
        {
            // log("❌ Data no updated", "ScannerUseCase.loop()");
        }
    }

    bleScannerManager->loop();
    if (bleScannerManager->isConnected() && !elm327BleManager->isConnected())
    {
        elm327BleManager->begin();
    }
    elm327BleManager->loop();

    if (elm327BleManager->hasDataAvailable())
    {
        String data =
            elm327BleManager->getJSON();

        if (data.compareTo(currentScannerData))
        {
            log("✅ Has Data updated: " + data, "ScannerUseCase.loop()");
            currentScannerData = data;
            bleManager->updateScannerData(data);
        }
        else
        {
            // log("❌ Data no updated", "ScannerUseCase.loop()");
        }
    }
}