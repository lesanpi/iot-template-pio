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
#include <BLEScannerManager.h>

class ScannerUseCase
{
    /* data */
public:
    ScannerUseCase(
        ELM327Manager *elm327Manager,
        ELM327Manager *elm327BleManager,
        BLEManager *bleManager,
        WiFiScannerManager *wifiScannerManager,
        BTScannerManager *bleScannerManager);

    void begin();
    void loop();

private:
    ELM327Manager *elm327Manager;
    ELM327Manager *elm327BleManager;
    BLEManager *bleManager;
    WiFiScannerManager *wifiScannerManager;
    BTScannerManager *bleScannerManager;
    unsigned long lastUpdateTime = 0;
    unsigned long lastConnectionObd2 = 0;
    // String currentScannerData = "";
};
#endif