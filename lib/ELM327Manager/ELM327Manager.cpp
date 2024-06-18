#include "ELM327Manager.h"

ELM327Manager::ELM327Manager(WiFiClient &client, bool silent, unsigned long timeout, bool useMock) : client(client), silent(!silent), timeout(timeout)
{
    this->useMock = useMock;
    log("Use mock " + String(useMock), "ELM327Manager");
}

void ELM327Manager::extractData()
{

    if (useMock)
    {
        delay(500);
        return;
    }
    switch (currentQuery)
    {
    case VIN:
    {
        if (!isGettingMessage())
            log("Extracting VIN", "ELM327Manager.extractData");

        vin();
        break;
    }
    case ENG_COOLANT_TEMP:
    {
        if (!isGettingMessage())
            log("ENG_COOLANT_TEMP", "ELM327Manager.extractData");
        engineCoolantTemp();
        break;
    }
    case ENG_OIL_TEMP:
    {
        if (!isGettingMessage())
            log("ENG_OIL_TEMP", "ELM327Manager.extractData");
        engineOilTemp();
        break;
    }
    case TIME_MIL_ON:
    {
        if (!isGettingMessage())
            log("TIME_MIL_ON", "ELM327Manager.extractData");
        timeMILOn();
        break;
    }
    case MIL_CODES:
    {
        if (!isGettingMessage())
            log("MIL_CODES", "ELM327Manager.extractData");
        getMILCodes();
        break;
    }
    case DIST_MIL_ON:
    {
        if (!isGettingMessage())
            log("DIST_MIL_ON", "ELM327Manager.extractData");
        distanceMILOn();
        break;
    }
    case FUEL_PRESSURE_KPA:
    {
        if (!isGettingMessage())
            log("FUEL_PRESSURE_KPA", "ELM327Manager.extractData");
        fuelPressure();
        break;
    }
    case ABS_BARO_PRESSURE:
    {
        if (!isGettingMessage())
            log("ABS_BARO_PRESSURE", "ELM327Manager.extractData");
        absBaroPressure();
        break;
    }
    case COMMANDED_EGR_PERCENTAGE:
    {
        if (!isGettingMessage())
            log("COMMANDED_EGR_PERCENTAGE", "ELM327Manager.extractData");
        commandedEGR();
        break;
    }
    case EGR_ERROR_PERCENTAGE:
    {
        if (!isGettingMessage())
            log("EGR_ERROR_PERCENTAGE", "ELM327Manager.extractData");
        errorEGR();
        break;
    }
    case MAF:
    {
        if (!isGettingMessage())
            log("MAF", "ELM327Manager.extractData");
        maf();
        break;
    }
    case INTAKE_AIR_TEMP_C:
    {
        if (!isGettingMessage())
            log("INTAKE_AIR_TEMP", "ELM327Manager.extractData");
        intakeAirTemp();
        break;
    }
    case MANIFOLD_PRESSURE_KPA:
    {
        if (!isGettingMessage())
            log("MANIFOLD_PRESSURE", "ELM327Manager.extractData");
        manifoldPressure();
        break;
    }
    case IS_MIL_ON:
    {
        if (!isGettingMessage())
            log("IS_MIL_ON", "ELM327Manager.extractData");
        isMILOn();
        break;
    }
    default:
    {
        nextQuery();
        break;
    }
    }
}

void ELM327Manager::begin(unsigned long timeout)
{

    if (!connected && client.connect(IPAddress(192, 168, 0, 10), 35000))
    {
        connected = true;
        log("Connected to ELM327 server", "ELM327Manager");
    }
    else
    {
        // return;
    }
    if (connected && !clientInitialized)
    {
        clientInitialized = elm327Client.begin(client, false, 10000);
        if (!clientInitialized)
        {
            log("Init elm client failed", "ELM327Manager");
            WiFi.reconnect();
            begin();
        }
    }
    initialized = elm327Client.initializeELM((char)48, 10000);

    log("Connection finalized correctly. Result: " + String(clientInitialized) + " Result init: " + String(initialized), "ELM327Manager");
}

void ELM327Manager::absBaroPressure()
{
    float absBaroPressure = elm327Client.absBaroPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Abs Baro Pressure is: " + String(absBaroPressure) + " kpa", "ELM327Manager");
        absBaroPressureValue = absBaroPressure;
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {

        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
        if (elm327Client.nb_rx_state == ELM_TIMEOUT)
        {
            disconnected();
            begin();
        }
    }
}

void ELM327Manager::fuelPressure()
{
    float fuelPressure = elm327Client.fuelPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Fuel Pressure: " + String(fuelPressure) + "%", "ELM327Manager");
        fuelPressureValue = fuelPressure;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {

        log("Error getting fuel pressure", "ELM327Manager.fuelLevel");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::engineCoolantTemp()
{
    float tempValue = elm327Client.engineCoolantTemp();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Engine Coolant Temperature: " + String(tempValue) + "°C", "ELM327Manager.engineCoolantTemp");
        engineCoolantTempValue = tempValue;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting engine coolant temp", "ELM327Manager.engineCoolantTemp");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::engineOilTemp()
{
    float tempValue = elm327Client.oilTemp();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Engine Oil Temperature: " + String(tempValue) + "°C", "ELM327Manager");
        engineOilTempValue = tempValue;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting engine oil temp", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::timeMILOn()
{
    int timeRunWithMIL = elm327Client.timeRunWithMIL();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("timeMILOn: " + String(timeRunWithMIL) + "min", "ELM327Manager");
        timeRunWithMILValue = timeRunWithMIL;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting timeMILOn", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::distanceMILOn()
{
    int distanceMILOn = elm327Client.distTravelWithMIL();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("distanceMILOn: " + String(distanceMILOn) + "km", "ELM327Manager");
        distanceRunWithMILValue = distanceMILOn;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting distanceMILOn", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::commandedEGR()
{
    float commandedEGR = elm327Client.commandedEGR();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("commandedEGR: " + String(commandedEGR) + "%", "ELM327Manager");
        commandedEGRValue = commandedEGR;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting commandedEGR", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::errorEGR()
{
    float errorEGR = elm327Client.egrError();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("errorEGR: " + String(errorEGR) + "%", "ELM327Manager");
        errorEGRValue = errorEGR;
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting errorEGR", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::maf()
{
    float maf = elm327Client.mafRate();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("maf: " + String(maf) + " g/s", "ELM327Manager");
        mafValue = maf;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting maf", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::vin()
{
    char vin[18];
    int8_t response = elm327Client.get_vin_blocking(vin);

    if (response == ELM_SUCCESS)
    {
        log("VIN: " + String(vin), "ELM327Manager");
        vinValue = vin;

        nextQuery();
    }
    else
    {
        log("Error getting VIN", "ELM327Manager");
        elm327Client.printError();
        nextQuery();
    }
}

void ELM327Manager::intakeAirTemp()
{
    float intakeAirTemp = elm327Client.intakeAirTemp();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("intakeAirTemp: " + String(intakeAirTemp) + " C", "ELM327Manager");
        intakeAirTempValue = intakeAirTemp;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting intakeAirTemp", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::manifoldPressure()
{
    float manifoldPressure = elm327Client.manifoldPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("manifoldPressure: " + String(manifoldPressure) + " C", "ELM327Manager");
        manifoldPressureValue = manifoldPressure;
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting manifoldPressure", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::isMILOn()
{
    elm327Client.monitorStatus();
    // elm327Client.commandedSecAirStatus();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        uint8_t milStatusRespose = (elm327Client.responseByte_3 & 0x80);
        log("Response Byte0: " + String(elm327Client.responseByte_0), "ELM327Manager");
        log("Response Byte1: " + String(elm327Client.responseByte_1), "ELM327Manager");
        log("Response Byte2: " + String(elm327Client.responseByte_2), "ELM327Manager");
        log("Response Byte3: " + String(elm327Client.responseByte_3), "ELM327Manager");

        numCodes = (elm327Client.responseByte_3 - 0x80);
        if (milStatusRespose)
        {
            log("MIL ON.", "ELM327Manager");
            milStatus = true;
        }
        else
        {
            log("MIL OFF.", "ELM327Manager");
            milStatus = false;
        }
        log("isMILOn: " + String(milStatus), "ELM327Manager");
        log("numCodesMIL: " + String(numCodes), "ELM327Manager");

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting isMILOn", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}

void ELM327Manager::getMILCodes()
{
    if (numCodes <= 0)
        return;

    elm327Client.currentDTCCodes(false);

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Codes found: " + String(elm327Client.DTC_Response.codesFound), "ELM327Manager.codesFound");

        for (int i = 0; i < elm327Client.DTC_Response.codesFound + 1; i++)
        {
            log(elm327Client.DTC_Response.codes[i], "ELM327Manager.CODE_FOUND");
        }
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting isMILOn", "ELM327Manager");
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}