
#ifndef ELM_327_MANAGER_H
#define ELM_327_MANAGER_H
#include <WiFi.h>
#include "Development.h"
#include "ELMduino.h"
#include <ArduinoJson.h>
#include <BluetoothSerial.h>
#include <BLEScannerManager.h>

typedef enum
{
    ELM_BLE,
    ELM_WIFI
} ELM_Manager_Type;

typedef enum
{
    ENG_OIL_TEMP,               // Check ❌
    ENG_COOLANT_TEMP,           // Check ✅
    MILEAGE,                    // Check ✅
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
    MAF,                        // Check ✅
    VIN,                        // Check ❌
    INTAKE_AIR_TEMP_C,          // Check ✅
    MANIFOLD_PRESSURE_KPA,      // Check ✅
    SPEED,                      // No relevant
} ELM_Manager_Query;
class ELM327Manager
{
public:
    // Constructor
    ELM327Manager(WiFiClient &client, BluetoothSerial &bleSerial, bool silent = false, unsigned long timeout = 10000, bool useMock = false, ELM_Manager_Type type = ELM_WIFI);

    String getName()
    {
        if (isBLEType())
            return "BLE - ELM327Manager";

        return "WiFi - ELM327Manager";
    }

    /// Connection Type Wifi or BLE
    ELM_Manager_Type type;

    bool isBLEType()
    {
        return type == ELM_BLE;
    }

    bool isWiFiType()
    {
        return type == ELM_WIFI;
    }

    // Initialize ELM327 communication
    void begin(unsigned long timeout = 10000);

    bool isConnected()
    {
        if (isBLEType())
        {
            return connected && clientInitialized && initializedElm327 && bleSerial.connected();
        }
        return connected && clientInitialized && initializedElm327 && WiFi.isConnected() && client.connected();
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
        // if (!connected)
        //     return;
        log("Disconnected from ELM327", getName() + ".disconnected()");
        bleSerial.disconnect();
        hasData = false;
        connected = false;
        clientInitialized = false;
        initializedElm327 = false;
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
        int timeToWait = isGettingMessage() ? 200 : 3000;
        // int timeToWait = 60000;
        if (now - lastDataExtractionTime >= timeToWait)
        {

            if (!isGettingMessage())
            {

                // delay(1000);
                log("Data Extraction...", getName() + "loop()");
            }
            extractData();
            // Update last connect time
            lastDataExtractionTime = now;
        }
    }

    /// @brief return if the manager has data
    /// @returns has data available for toJson()
    bool hasDataAvailable()
    {
        if (useMock)
            return true;
        return hasData;
    }

    /// @brief  create JSON string with non-null data
    /// @return Stringify JSON with data
    String getJSON()
    {
        // Allocate a dynamic JSON document with enough memory
        JsonDocument doc;

        // Create a JsonObject object
        JsonObject obj = doc.to<JsonObject>();

        obj["vehicle"] = vehicleId;
        // Use mock data if useMock is true
        if (useMock)
        {
            delay(1000);
            // Generate mock data for all fields
            obj["vin"] = "JTDKW923572004448";
            obj["maf"] = 0.23;
            obj["mileage"] = 100000;
            obj["absBaroPressure"] = 93;
            obj["fuelPressure"] = 80;
            obj["coolantTemperature"] = 27;
            obj["oilTemperature"] = 30;
            obj["minutesMILOn"] = 40;
            obj["kmMILOn"] = 300;
            obj["milOn"] = true;
            obj["commandedEGR"] = 20;
            obj["errorEGR"] = 20;
            obj["intakeAirTemperature"] = 30;
            obj["manifoldPressureKpa"] = 60;

            // Handle mock DTC codes (assuming random number of codes)
            JsonArray dtcArray = obj.createNestedArray("milCodes");
            int numMockCodes = rand() % 16 + 1; // Generate 1-16 random codes
            obj["manifoldPressure"] = 60;

            for (int i = 0; i < numMockCodes; i++)
            {
                dtcArray.add("P0031");
            }
            String jsonString;
            serializeJson(doc, jsonString);
            return jsonString;
        }

        obj["numCodes"] = numCodes;
        obj["milStatus"] = milStatus;
        obj["milOn"] = milStatus;

        if (vinValue != "")
        {
            obj["vin"] = vinValue;
        }
        if (mafValue >= 0)
        { // Check for NaN values
            obj["maf"] = mafValue;
        }
        if (absBaroPressureValue >= 0)
        {
            obj["absBaroPressure"] = absBaroPressureValue;
        }
        if (fuelPressureValue >= 0)
        {
            obj["fuelPressure"] = fuelPressureValue;
        }
        if (engineCoolantTempValue >= 0)
        {
            obj["coolantTemperature"] = engineCoolantTempValue;
        }
        if (engineOilTempValue >= 0)
        {
            obj["oilTemperature"] = engineOilTempValue;
        }

        if (timeRunWithMILValue >= 0)
        {
            obj["minutesMILOn"] = timeRunWithMILValue;
        }
        if (distanceRunWithMILValue >= 0)
        {
            obj["kmMILOn"] = distanceRunWithMILValue;
        }
        if (mileageValue >= 0)
        {
            obj["mileage"] = mileageValue;
        }
        if (commandedEGRValue >= 0)
        {
            obj["commandedEGR"] = commandedEGRValue;
        }
        if (errorEGRValue >= 0)
        {
            obj["errorEGR"] = errorEGRValue;
        }
        if (intakeAirTempValue >= 0)
        {
            obj["intakeAirTemperature"] = intakeAirTempValue;
        }
        if (manifoldPressureValue >= 0)
        {
            obj["manifoldPressureKpa"] = manifoldPressureValue;
        }

        // Handle DTC codes (assuming each code is a separate string)
        // JsonArray dtcArray = obj["milCodes"].to<JsonArray>();
        // for (int i = 0; i < 16; i++)
        // {
        //     if (dtcCodesValue[i][0] != '\0')
        //     { // Check if code is not empty
        //         dtcArray.add(dtcCodesValue[i]);
        //     }
        // }

        // Serialize the JSON document into a String
        String jsonString;

        serializeJson(doc, jsonString);

        return jsonString;
    }

    /// @brief set Vehicle Id
    void setVehicleId(String id)
    {
        this->vehicleId = id;
    }

private:
    /// @brief Vehicle id
    String vehicleId = "";

    /// @brief Data have been extracted
    bool hasData = false;

    /// @brief Use mock data
    bool useMock;

    /// @brief WiFi client
    WiFiClient &client;

    /// @brief BLE Serial
    BluetoothSerial &bleSerial;

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
    bool initializedElm327 = false;

    /// @brief Timeout
    unsigned long timeout;

    /// @brief Last time execution of data scrapping
    unsigned long lastDataExtractionTime = 0;

    /// @brief  Num codes of MIL on
    uint8_t numCodes = 0;

    /// @brief  MIL light is on
    bool milStatus = 0;

    /// @brief VIN
    String vinValue = "";

    /// @brief  MAF
    float mafValue = -1;

    /// @brief Mileage odometer value
    float mileageValue = -1;

    /// @brief Abs Baro pressure
    float absBaroPressureValue = -1;

    /// @brief Fuel pressure
    float fuelPressureValue = -1;

    /// @brief Engine coolant temperature
    float engineCoolantTempValue = -1;

    /// @brief Engine oil temperature
    float engineOilTempValue = -1;

    /// @brief Time with MIL on in minutes
    int timeRunWithMILValue = 0;

    /// @brief Distance with MIL on in minutes
    int distanceRunWithMILValue = 0;

    /// @brief commandedEGR value
    float commandedEGRValue = -1;

    /// @brief errorEGR value
    float errorEGRValue = -1;

    /// @brief intakeAirTemp value
    float intakeAirTempValue = -1;

    /// @brief manifoldPressure value
    float manifoldPressureValue = -1;

    /// @brief DTC codes values
    char dtcCodesValue[16][6];

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
            return MILEAGE;
        case MILEAGE:
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
            hasData = true;
            log("✅ Has Data updated", getName() + ".getNextQuery()");
            return ENG_OIL_TEMP; // Reset to the beginning
        }
    }

    void printError()
    {
        if (!silent)
        {
            log("ELM327 Error!", getName());
        }
    }

    void odometer();
    uint8_t getOdometerValue();
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