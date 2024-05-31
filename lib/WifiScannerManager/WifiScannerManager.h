#ifndef WIFI_SCANNER_MANAGER_H
#define WIFI_SCANNER_MANAGER_H
#include <WiFi.h>
#include "Development.h"

class WiFiScannerManager
{
public:
    WiFiScannerManager(const char *ssid, int minRSSI)
    {
        this->ssid = ssid;
        this->minRSSI = minRSSI;
    }
    void begin(){
   
    }

    void connect();
    void loop() {
        // Time in milliseccond
        unsigned long now = millis();

        // Check a minute has passed since the last run
        if (now - lastConnectTime >= 60000) {
            // Actualiza el tiempo de la última ejecución
            lastConnectTime = now;

            // Ejecuta la función connect
            connect();
        }
    }

    void disconnected(){
        log("Disconnected from WiFI", "WiFiScannerManager.onEvent()");
        connected = false;
    }


private:
    // WiFi name to search
    const char *ssid;
    // Min RSSI for connection
    int minRSSI;

    bool connected = false;

    /// Last time of execution of `connect()`
    unsigned long lastConnectTime = 0; 

    /// on Wifi Event occurred
    void onWiFiEvent(WiFiEvent_t event) {
        if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
            log("Disconnected from WiFI", "WiFiScannerManager.onEvent()");
            connected = false;
        }
    }
    
    
};

#endif