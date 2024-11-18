#include "ELM327Manager.h"

ELM327Manager::ELM327Manager(WiFiClient &client, BluetoothSerial &bleSerial, bool silent, unsigned long timeout, bool useMock, ELM_Manager_Type type) : client(client), bleSerial(bleSerial), silent(!silent), timeout(timeout)
{
    this->useMock = useMock;
    this->type = type;
    log("Use mock " + String(useMock), getName());
}

void ELM327Manager::odometer()
{
    int mileage = getOdometerValue();
    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Odometer mileage is: " + String(mileage) + " Km", getName());
        mileageValue = mileage;
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
            // begin();
        }
    }
}

uint8_t ELM327Manager::getOdometerValue()
{
    log("Get Odometer", getName());
    return (uint8_t)elm327Client.processPID(SERVICE_01, 166, 1, 4, 0.1);
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
            log("Extracting VIN", getName() + ".extractData");

        vin();
        break;
    }
    case ENG_COOLANT_TEMP:
    {
        if (!isGettingMessage())
            log("ENG_COOLANT_TEMP", getName() + ".extractData");
        engineCoolantTemp();
        break;
    }
    case MILEAGE:
    {
        if (!isGettingMessage())
            log("MILEAGE", getName() + ".extractData");
        odometer();
        break;
    }
    case ENG_OIL_TEMP:
    {
        if (!isGettingMessage())
            log("ENG_OIL_TEMP", getName() + ".extractData");
        engineOilTemp();
        break;
    }
    case TIME_MIL_ON:
    {
        if (!isGettingMessage())
            log("TIME_MIL_ON", getName() + ".extractData");
        timeMILOn();
        break;
    }
    case MIL_CODES:
    {
        if (!isGettingMessage())
            log("MIL_CODES", getName() + ".extractData");
        getMILCodes();
        break;
    }
    case DIST_MIL_ON:
    {
        if (!isGettingMessage())
            log("DIST_MIL_ON", getName() + ".extractData");
        distanceMILOn();
        break;
    }
    case FUEL_PRESSURE_KPA:
    {
        if (!isGettingMessage())
            log("FUEL_PRESSURE_KPA", getName() + ".extractData");
        fuelPressure();
        break;
    }
    case ABS_BARO_PRESSURE:
    {
        if (!isGettingMessage())
            log("ABS_BARO_PRESSURE", getName() + ".extractData");
        absBaroPressure();
        break;
    }
    case COMMANDED_EGR_PERCENTAGE:
    {
        if (!isGettingMessage())
            log("COMMANDED_EGR_PERCENTAGE", getName() + ".extractData");
        commandedEGR();
        break;
    }
    case EGR_ERROR_PERCENTAGE:
    {
        if (!isGettingMessage())
            log("EGR_ERROR_PERCENTAGE", getName() + ".extractData");
        errorEGR();
        break;
    }
    case MAF:
    {
        if (!isGettingMessage())
            log("MAF", getName() + ".extractData");
        maf();
        break;
    }
    case INTAKE_AIR_TEMP_C:
    {
        if (!isGettingMessage())
            log("INTAKE_AIR_TEMP", getName() + ".extractData");
        intakeAirTemp();
        break;
    }
    case MANIFOLD_PRESSURE_KPA:
    {
        if (!isGettingMessage())
            log("MANIFOLD_PRESSURE", getName() + ".extractData");
        manifoldPressure();
        break;
    }
    case IS_MIL_ON:
    {
        if (!isGettingMessage())
            log("IS_MIL_ON", getName() + ".extractData");
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

    if (isBLEType())
    {

        if (!connected && bleSerial.connected())
        {
            connected = true;
            log("Connected to ELM327 BLE", getName());
        }
        if (connected && !clientInitialized)
        {
            clientInitialized = elm327Client.begin(bleSerial, false, timeout);
            if (!clientInitialized)
            {
                log("Init elm client failed", getName());
                return;
            }
        }
        initializedElm327 = elm327Client.initializeELM((char)48, timeout);
        log("Connection finalized correctly. Result: " + String(clientInitialized) + " Result init: " + String(initializedElm327), getName());
        return;
    }

    if (!connected && client.connect(IPAddress(192, 168, 0, 10), 35000))
    {
        connected = true;
        log("Connected to ELM327 server", getName());
    }
    else
    {
        connected = false;
        return;
    }

    if (connected && !clientInitialized)
    {
        clientInitialized = elm327Client.begin(client, false, timeout);
        if (!clientInitialized)
        {
            log("Init elm client failed", getName());
            connected = false;
            // WiFi.reconnect();
            // begin();
        }
    }
    initializedElm327 = elm327Client.initializeELM((char)48, int(timeout));
    log("Connection finalized correctly. Result: " + String(clientInitialized) + " Result init: " + String(initializedElm327), getName());
}

void ELM327Manager::absBaroPressure()
{
    float absBaroPressure = elm327Client.absBaroPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Abs Baro Pressure is: " + String(absBaroPressure) + " kpa", getName());
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
            // begin();
        }
    }
}

void ELM327Manager::fuelPressure()
{
    float fuelPressure = elm327Client.fuelPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Fuel Pressure: " + String(fuelPressure) + "%", getName());
        fuelPressureValue = fuelPressure;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {

        log("Error getting fuel pressure", getName() + ".fuelLevel");
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
        log("Engine Coolant Temperature: " + String(tempValue) + "°C", getName() + ".engineCoolantTemp");
        engineCoolantTempValue = tempValue;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting engine coolant temp", getName() + ".engineCoolantTemp");
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
        log("Engine Oil Temperature: " + String(tempValue) + "°C", getName());
        engineOilTempValue = tempValue;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting engine oil temp", getName());
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
        log("timeMILOn: " + String(timeRunWithMIL) + "min", getName());
        timeRunWithMILValue = timeRunWithMIL;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting timeMILOn", getName());
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
        log("distanceMILOn: " + String(distanceMILOn) + "km", getName());
        distanceRunWithMILValue = distanceMILOn;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting distanceMILOn", getName());
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
        log("commandedEGR: " + String(commandedEGR) + "%", getName());
        commandedEGRValue = commandedEGR;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting commandedEGR", getName());
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
        log("errorEGR: " + String(errorEGR) + "%", getName());
        errorEGRValue = errorEGR;
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting errorEGR", getName());
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
        log("maf: " + String(maf) + " g/s", getName());
        mafValue = maf;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting maf", getName());
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
        log("VIN: " + String(vin), getName());
        vinValue = vin;

        nextQuery();
    }
    else
    {
        log("Error getting VIN", getName());
        elm327Client.printError();
        nextQuery();
    }
}

void ELM327Manager::intakeAirTemp()
{
    float intakeAirTemp = elm327Client.intakeAirTemp();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("intakeAirTemp: " + String(intakeAirTemp) + " C", getName());
        intakeAirTempValue = intakeAirTemp;

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting intakeAirTemp", getName());
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
        log("manifoldPressure: " + String(manifoldPressure) + " C", getName());
        manifoldPressureValue = manifoldPressure;
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting manifoldPressure", getName());
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
        log("Response Byte0: " + String(elm327Client.responseByte_0), getName());
        log("Response Byte1: " + String(elm327Client.responseByte_1), getName());
        log("Response Byte2: " + String(elm327Client.responseByte_2), getName());
        log("Response Byte3: " + String(elm327Client.responseByte_3), getName());

        numCodes = (elm327Client.responseByte_3 - 0x80);
        if (milStatusRespose)
        {
            log("MIL ON.", getName());
            milStatus = true;
        }
        else
        {
            log("MIL OFF.", getName());
            milStatus = false;
        }
        log("isMILOn: " + String(milStatus), getName());
        log("numCodesMIL: " + String(numCodes), getName());

        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting isMILOn", getName());
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
        log("Codes found: " + String(elm327Client.DTC_Response.codesFound), getName() + ".codesFound");

        for (int i = 0; i < elm327Client.DTC_Response.codesFound + 1; i++)
        {
            log(elm327Client.DTC_Response.codes[i], getName() + ".CODE_FOUND");
        }
        nextQuery();
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting isMILOn", getName());
        elm327Client.printError();
        /// Continue with next query if manager is not getting message
        nextQuery();
    }
}