
#ifndef ELM_327_MANAGER_H
#define ELM_327_MANAGER_H
#include <WiFi.h>
#include "Development.h"
#include "ELMduino.h"

class ELM327Manager
{
public:
    // Constructor
    ELM327Manager(WiFiClient &client, bool silent = false, unsigned long timeout = 10000);

    // Initialize ELM327 communication
    void begin(unsigned long timeout = 10000);

    // Read absolute Barometric Pressure (replace with desired function)
    float absBaroPressure();
    float engineCoolantTemp();
    float fuelLevel();

    bool isConnected()
    {
        return connected;
    }

    void disconnected()
    {
        if (!connected)
            return;
        log("Disconnected from ELM327", "ELM327.disconnected()");
        connected = false;
    }
    void loop()
    {

        // Time in milliseccond
        unsigned long now = millis();

        // Check a minute has passed since the last run
        if (now - lastDataExtractionTime >= 60000 * 3)
        {
            if (!isConnected())
            {
                log("ELM327 is not Connected", "ELM327.loop()");
                return;
            }
            // Update last connectTme
            lastDataExtractionTime = now;
            log("Data Extraction...", "ELM327.loop()");
            yield();
            elm327Client.flushInputBuff();

            absBaroPressure();
            // engineCoolantTemp();
            // fuelLevel();
        }
    }

private:
    /// WiFi client
    WiFiClient &client;

    ELM327 elm327Client;

    /// Silent print errors
    bool silent;
    /// Manager is connected to a ELM327
    bool connected = false;
    unsigned long timeout;

    /// Last time execution of data scrapping
    unsigned long lastDataExtractionTime = 0;

    void printError()
    {
        if (!silent)
        {
            log("ELM327 Error!", "ELM327Manager");
        }
    }
};

#endif