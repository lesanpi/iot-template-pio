#include "ScannerUseCase.h"

ScannerUseCase::ScannerUseCase(ELM327Manager *elm327Manager, BLEManager *bleManager, WiFiScannerManager *wifiScannerManager)
{
    this->elm327Manager = elm327Manager;
    this->wifiScannerManager = wifiScannerManager;
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
}