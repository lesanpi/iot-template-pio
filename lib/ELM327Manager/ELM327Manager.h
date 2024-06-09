
#ifndef ELM_327_MANAGER_H
#define ELM_327_MANAGER_H
#include <WiFi.h>
#include "Development.h"
#include "ELMduino.h"
typedef enum
{
    ENG_RPM,
    ENG_COOLANT_TEMP,
    ENG_OIL_TEMP,
    TIME_MIL_ON,
    DIST_MIL_ON,
    FUEL_PRESSURE,
    ABS_BARO_PRESSURE,
    COMMANDED_EGR,
    EGR_ERROR,
    DISTANCE_SINCE_DTC_CLEARED,
    MAF,
    VIN,
    SPEED,
} manager_obd_query;
class ELM327Manager
{
public:
    // Constructor
    ELM327Manager(WiFiClient &client, bool silent = false, unsigned long timeout = 10000);

    // Initialize ELM327 communication
    void begin(unsigned long timeout = 10000);

    // Read absolute Barometric Pressure (replace with desired function)
    void absBaroPressure();
    void engineCoolantTemp();
    void fuelLevel();

    bool isConnected()
    {
        return connected && clientInitialized && initialized;
    }

    bool isGettingMessage()
    {
        return elm327Client.nb_rx_state == ELM_GETTING_MSG;
    }

    void disconnected()
    {
        if (!connected)
            return;
        log("Disconnected from ELM327", "ELM327.disconnected()");
        connected = false;
        clientInitialized = false;
        initialized = false;
    }
    void loop()
    {

        // Time in milliseccond
        unsigned long now = millis();

        // Check a minute has passed since the last run
        int timeToWait = isGettingMessage() ? 1000 : 10000;
        if (now - lastDataExtractionTime >= timeToWait)
        {
            if (!isConnected())
            {
                // log("ELM327 is not Connected", "ELM327.loop()");
                return;
            }
            // Update last connectTme
            lastDataExtractionTime = now;
            log("Data Extraction...", "ELM327.loop()");
            yield();

            absBaroPressure();
            // elm327Client.flushInputBuff();
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
    /// Client ELM327 initialized
    bool clientInitialized = false;
    /// Elm327 is initilized
    bool initialized = false;
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