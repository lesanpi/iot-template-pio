#include "WiFiScannerManager.h"

void WiFiScannerManager::connect()
{
    if (WiFi.isConnected())
    {
        log("Ya esta conectado a una red WiFi.", "WiFiScannerManager.connect()");
        return;
    }
    // Escanea redes WiFi disponibles
    int scanResultsCount = WiFi.scanNetworks();

    // Recorre las redes encontradas
    for (int i = 0; i < scanResultsCount; i++)
    {
        // Obtiene la información de la red actual
        String ssid = WiFi.SSID(i);
        int resultRSSI = WiFi.RSSI(i);
        log("🧭 WiFi Scanned!: " + ssid + " with RSSI: " + String(resultRSSI), "WiFiScannerManagerManager.connect()");

        // Comprueba si coincide con el SSID deseado
        for (int j = 0; j < ssidList.numSsids; j++)
        {
            if (strcmp(ssid.c_str(), ssidList.ssids[j]) == 0)
            {
                log("🧭 WiFi FOUND!", "WiFiScannerManagerManager.connect()");

                // Verifica si el RSI es lo suficientemente alto
                if (resultRSSI >= minRSSI)
                {
                    // Trying to connect
                    log("Start connection with: " + ssid + " with RSSI: " + String(resultRSSI), "WiFiScannerManagerManager.connect()");
                    WiFi.begin(ssid.c_str());

                    // Espera a que la conexión se establezca
                    while (WiFi.status() != WL_CONNECTED)
                    {
                        delay(500);
                        Serial.print(".");
                    }

                    // Success
                    log("✅ WiFi conectado!", "WiFiScannerManager.connect()");
                    connected = true;

                    return;
                }
            }
        }
    }

    // No se encontró la red WiFi o el RSI no era lo suficientemente alto
    log("No se pudo conectar a WiFi.", "WiFiScannerManager.connect()");
}