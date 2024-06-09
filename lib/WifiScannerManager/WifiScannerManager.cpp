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
        // WiFiScanResult result = WiFi.scanResults(i);
        String resultSSID = WiFi.SSID(i);
        int resultRSSI = WiFi.RSSI(i);
        log("🧭 WiFi Scanned!: " + resultSSID + " with RSSI: " + String(resultRSSI), "WiFiScannerManagerManager.connect()");

        // Comprueba si coincide con el SSID deseado
        if (strcmp(resultSSID.c_str(), ssid) == 0)
        {
            log("🧭 WiFi FOUND!", "WiFiScannerManagerManager.connect()");

            // Verifica si el RSI es lo suficientemente alto
            if (resultRSSI >= minRSSI)
            {
                // Trying to connect
                log("Start connection with: " + resultSSID + " with RSSI: " + String(resultRSSI), "WiFiScannerManagerManager.connect()");
                WiFi.begin(ssid);

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

    // No se encontró la red WiFi o el RSI no era lo suficientemente alto
    log("No se pudo conectar a WiFi.", "WiFiScannerManager.connect()");
}