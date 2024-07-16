#ifndef BLE_SCANNER_MANAGER_H
#define BLE_SCANNER_MANAGER_H
#include "Development.h"
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BluetoothSerial.h>

// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
// {
//     void onResult(BLEAdvertisedDevice advertisedDevice)
//     {

//         Serial.printf("Advertised Device: %s, %s RSSI: %d \n", advertisedDevice.getName().c_str(), advertisedDevice.getAddress().toString().c_str(), advertisedDevice.getRSSI());
//         Serial.printf("Services UUIS: %d\n", advertisedDevice.getServiceUUIDCount());
//         // Imprimir los UUIDs de servicios
//         // if (advertisedDevice.getServiceData().size() > 0)
//         // {
//         //     Serial.println("UUIDs de servicios:");
//         //     for (BLEUUID uuid : advertisedDevice.getServiceUUID())
//         //     {
//         //         Serial.print(" - ");
//         //         log(uuid.toString().c_str(), "BLEAdvertisedDevice.serviceUUIDFound");
//         //     }
//         // }
//         // else
//         // {
//         //     Serial.println("No se encontraron UUIDs de servicios");
//         // }
//     }
// };

class BLEScannerManager
{

public:
    // Constructor de la clase
    BLEScannerManager(const char *bleName, BluetoothSerial &bleSerial, int minRSSI);

    void loop()
    {
        // Time in milliseccond
        unsigned long now = millis();

        // Check a minute has passed since the last run
        if (now - lastConnectTime >= 10000)
        {
            // Actualiza el tiempo de la última ejecución
            lastConnectTime = now;
            scanDevices();
        }
    }

    int SCAN_TIME = 20; /// Seconds

    // Scan and connect BLE Devices
    void scanDevices();

    void disconnected()
    {
        if (!connected)
            return;
        log("Disconnected from WiFI", "BLEScannerManager.disconnected()");
        bool success = bleSerial.disconnect();
        if (success)
        {
            connected = false;
        }
    }

    bool isConnected()
    {
        // return connected && bleSerial.connected();
        return bleSerial.connected();
    }

private:
    /// @brief BLE Serial
    BluetoothSerial &bleSerial;

    // BLE name to search
    const char *bleName;

    // Min RSSI for connection
    int minRSSI;

    /// Is connected
    bool connected = false;

    /// Last time of execution of `connect()`
    unsigned long lastConnectTime = 0;

    // or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE to request pincode confirmation
    esp_spp_sec_t sec_mask = ESP_SPP_SEC_ENCRYPT;

    // or ESP_SPP_ROLE_MASTER
    esp_spp_role_t role = ESP_SPP_ROLE_MASTER;
};
#endif