#ifndef WIFI_SCANNER_MANAGER_H
#define WIFI_SCANNER_MANAGER_H
#include <WiFi.h>
#include "Development.h"
struct SSIDList
{
    const char *ssids[4];
    int numSsids;
};

class WiFiScannerManager
{
public:
    WiFiScannerManager(SSIDList ssidListSearch, int minRSSI)
    {
        // this->ssid = ssid;
        ssidList = ssidListSearch;
        this->minRSSI = minRSSI;
    }
    void begin() {}

    void connect();
    void loop()
    {
        // Time in milliseccond
        unsigned long now = millis();

        // Check a minute has passed since the last run
        if (now - lastConnectTime >= 1000 * 60)
        {
            // Actualiza el tiempo de la última ejecución
            lastConnectTime = now;

            // Ejecuta la función connect
            connect();
        }
    }

    void disconnected()
    {
        if (!connected)
            return;
        log("Disconnected from WiFI", "WiFiScannerManager.onEvent()");
        connected = false;
        WiFi.disconnect();
    }

    bool isConnected()
    {
        return connected && WiFi.isConnected();
    }

    /// on Wifi Event occurred
    void onWiFiEvent(WiFiEvent_t event)
    {
        if (event == WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED)
        {
            log("Disconnected from WiFI", "WiFiScannerManager.onEvent()");
            connected = false;
        }
    }

private:
    /// @brief SSID List to search
    SSIDList ssidList;
    // Min RSSI for connection
    int minRSSI;

    bool connected = false;

    /// Last time of execution of `connect()`
    unsigned long lastConnectTime = 0;
};

#endif