
#ifndef ELM_327_MANAGER_H
#define ELM_327_MANAGER_H
#include <WiFi.h>
#include "Development.h"
#include "ELMduino.h"
typedef enum
{
    ENG_OIL_TEMP,               // Check ❌
    ENG_COOLANT_TEMP,           // Check ✅
    ABS_BARO_PRESSURE,          // Check ✅
    EGR_ERROR_PERCENTAGE,       // Check ❌
    COMMANDED_EGR_PERCENTAGE,   // Check ❌
    IS_MIL_ON,                  // Check ✅
    MIL_CODES,                  // Check ✅
    TIME_MIL_ON,                // Check ✅
    DIST_MIL_ON,                // Check ✅
    FUEL_PRESSURE_KPA,          // Check ❌
    ENG_RPM,                    // No relevant
    DISTANCE_SINCE_DTC_CLEARED, // No relevant
    MAF,                        // Check ❌
    VIN,                        // Check
    INTAKE_AIR_TEMP_C,          // Check
    MANIFOLD_PRESSURE_KPA,      // Check
    SPEED,                      // No relevant
} ELM_Manager_Query;
class ELM327Manager
{
public:
    // Constructor
    ELM327Manager(WiFiClient &client, bool silent = false, unsigned long timeout = 10000);

    // Initialize ELM327 communication
    void begin(unsigned long timeout = 10000);

    bool isConnected()
    {
        return connected && clientInitialized && initialized;
    }

    bool isGettingMessage()
    {
        return elm327Client.nb_rx_state == ELM_GETTING_MSG;
    }

    bool isTimeOut()
    {
        return elm327Client.nb_rx_state == ELM_TIMEOUT;
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

        /// Timeout, reconnect
        if (isTimeOut())
        {
            disconnected();
            begin();
            return;
        }

        if (!isConnected())
        {
            return;
        }

        // Time in milliseccond
        unsigned long now = millis();
        // Check a minute has passed since the last run
        int timeToWait = isGettingMessage() ? 200 : 10000;
        // int timeToWait = 60000;
        if (now - lastDataExtractionTime >= timeToWait)
        {

            if (!isGettingMessage())
            {

                // delay(1000);
                log("Data Extraction...", "ELM327.loop()");
            }
            extractData();
            // Update last connect time
            lastDataExtractionTime = now;
        }
    }

private:
    /// @brief WiFi client
    WiFiClient &client;

    /// @brief Client to ELM327 WiFi service
    ELM327 elm327Client;

    /// @brief Current query
    ELM_Manager_Query currentQuery = ELM_Manager_Query::ENG_OIL_TEMP;

    /// @brief Silent print errors
    bool silent;

    /// @brief Manager is connected to a ELM327
    bool connected = false;

    /// @brief Client ELM327 initialized
    bool clientInitialized = false;

    /// @brief Elm327 is initilized
    bool initialized = false;

    /// @brief Timeout
    unsigned long timeout;

    /// @brief Last time execution of data scrapping
    unsigned long lastDataExtractionTime = 0;

    /// @brief  Num codes of MIL on
    uint8_t numCodes = 0;

    /// @brief  MIL light is on
    bool milStatus = 0;

    /// Extract data
    void extractData();

    /// Continue with next query
    void nextQuery()
    {
        if (isGettingMessage())
            return;

        currentQuery = getNextQuery(currentQuery);
    }

    /// Get next query
    ELM_Manager_Query getNextQuery(ELM_Manager_Query currentQuery)
    {

        switch (currentQuery)
        {
        case ENG_OIL_TEMP:
            return ENG_COOLANT_TEMP;
        case ENG_COOLANT_TEMP:
            return ABS_BARO_PRESSURE;
        case ABS_BARO_PRESSURE:
            return EGR_ERROR_PERCENTAGE;
        case EGR_ERROR_PERCENTAGE:
            return COMMANDED_EGR_PERCENTAGE;
        case COMMANDED_EGR_PERCENTAGE:
            return IS_MIL_ON;
        case IS_MIL_ON:
            return MIL_CODES;
        case MIL_CODES:
            return TIME_MIL_ON;
        case TIME_MIL_ON:
            return DIST_MIL_ON;
        case DIST_MIL_ON:
            return FUEL_PRESSURE_KPA;
        case FUEL_PRESSURE_KPA:
            return ENG_RPM;
        case ENG_RPM:
            return MAF;
        case MAF:
            return VIN;
        case VIN:
            return INTAKE_AIR_TEMP_C;
        case INTAKE_AIR_TEMP_C:
            return MANIFOLD_PRESSURE_KPA;
        default:
            return ENG_OIL_TEMP; // Reset to the beginning
        }
    }

    void printError()
    {
        if (!silent)
        {
            log("ELM327 Error!", "ELM327Manager");
        }
    }

    void absBaroPressure();
    void engineCoolantTemp();
    void engineOilTemp();
    void fuelPressure();
    void timeMILOn();
    void distanceMILOn();
    void commandedEGR();
    void errorEGR();
    void maf();
    void vin();
    void intakeAirTemp();
    void manifoldPressure();
    void isMILOn();
    void getMILCodes();
};

#endif