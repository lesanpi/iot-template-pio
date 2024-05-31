#include "ELM327Manager.h"

ELM327Manager::ELM327Manager(WiFiClient &client, bool silent, unsigned long timeout) : client(client), silent(!silent), timeout(timeout) {}

void ELM327Manager::begin(unsigned long timeout)
{

    if (connected)
        return;
    if (client.connect(IPAddress(192, 168, 0, 10), 35000))
        log("Connected to ELM327 server", "ELM327Manager");
    else
        return;
    bool result = elm327Client.begin(client, true, 10000);
    bool resultInit = elm327Client.initializeELM();

    connected = true;
    log("Connection finalized correctly. Result: " + String(result) + " Result init: " + String(resultInit), "ELM327Manager");
}

float ELM327Manager::absBaroPressure()
{
    float tempRPM = elm327Client.absBaroPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Abs Baro Pressure is: " + String(tempRPM) + " kpa", "ELM327Manager.absBaroPressure");
        return tempRPM;
    }
    else
    {
        // printError();
        log("Abs Baro Pressure failed", "ELM327Manager.absBaroPressure");

        return tempRPM;
    }
}

float ELM327Manager::fuelLevel()
{
    float fuelPercent = elm327Client.fuelLevel(); // Replace with actual call if different

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Fuel Level: " + String(fuelPercent) + "%", "ELM327Manager");
        return fuelPercent;
    }
    else
    {
        printError(); // Assuming you have a printError function defined
        log("Abs Baro Pressure failed", "ELM327Manager.absBaroPressure");

        return -100.0f; // Or any default value on error
    }
}

float ELM327Manager::engineCoolantTemp()
{
    float tempValue = elm327Client.engineCoolantTemp(); // Replace with actual call if different

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Engine Coolant Temperature: " + String(tempValue) + "°C", "ELM327Manager");
        return tempValue;
    }
    else
    {
        printError(); // Assuming you have a printError function defined
    }
}
