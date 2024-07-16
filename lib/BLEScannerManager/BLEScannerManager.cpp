#include "BLEScannerManager.h"

BLEScannerManager::BLEScannerManager(const char *bleName, BluetoothSerial &bleSerial, int minRSSI) : bleSerial(bleSerial)
{
    {
        this->bleName = bleName;
        this->minRSSI = minRSSI;
    }
}

void BLEScannerManager::scanDevices()
{

    if (bleSerial.connected())
    {
        log("Ya esta conectado a un BLE Device.", "BLEScannerManager.scanDevices()");
        return;
    }
    // Almacenar los resultados del escaneo
    BLEScanResults results;

    // Esperar 5 segundos para el escaneo
    // sleep(5000);

    // Detener el escaneo
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->stop();
    // pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // Configurar la función de callback para el escaneo
    pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    pBLEScan->setInterval(0x50);
    pBLEScan->setWindow(0x30);
    Serial.printf("Start BLE scan for %d seconds...\n", SCAN_TIME);

    // Procesar los resultados del escaneo
    BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
    int count = foundDevices.getCount();

    for (int i = 0; i < count; i++)
    {
        BLEAdvertisedDevice d = foundDevices.getDevice(i);
        String resultName = d.getName().c_str();
        BLEAddress currDevAddr = d.getAddress();
        String currDevAddrStr = currDevAddr.toString().c_str();
        BLEUUID currDevUUID = d.getServiceUUID();
        int resultRSSI = d.getRSSI();
        log("Found device " + resultName + " with RSSI: " + d.getRSSI(), "BLEScannerManager.scanDevices()");
        // Serial.printf("Service UUID %s \n", currDevUUID.toString());
        // Comprof if the device name is equal to scanner search name
        if (strcmp(resultName.c_str(), bleName) == 0)

        {
            log("🧭 BLE FOUND!", "BLEScannerManager.scanDevices()");

            // Verify if the RSSI is bigger than minRSSI
            if (resultRSSI >= minRSSI)
            {
                // Trying to connect
                log("Start connection with: " + resultName + " with RSSI: " + String(resultRSSI), "BLEScannerManager.scanDevices()");
                bool bleConnected = bleSerial.connect(resultName);
                if (bleConnected)
                {
                    log("✅ BLE conectado!", "BLEScannerManager.scanDevices()");
                    connected = true;
                    return;
                }
                else
                {
                    log("❌ BLE connection failed!", "BLEScannerManager.scanDevices()");
                }
            }
        }
    }
}
