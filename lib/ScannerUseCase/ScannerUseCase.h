#ifndef SCANNER_USE_CASE_H
#define SCANNER_USE_CASE_H
#include "Development.h"
#include "GPSManager.h"
#include "MemoryManager.h"
#include "BLEManager.h"
#include "InputManager.h"
#include "OutputManager.h"
#include "ELM327Manager.h"
#include "WifiScannerManager.h"

class ScannerUseCase
{
    /* data */
public:
    ScannerUseCase(ELM327Manager *elm327Manager, BLEManager *bleManager, WiFiScannerManager *wifiScannerManager);

    void begin();
    void loop();

private:
    ELM327Manager *elm327Manager;
    BLEManager *bleManager;
    WiFiScannerManager *wifiScannerManager;
    String currentScannerData = "";
};
#endif