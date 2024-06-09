#include "ELM327Manager.h"

ELM327Manager::ELM327Manager(WiFiClient &client, bool silent, unsigned long timeout) : client(client), silent(!silent), timeout(timeout) {}

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
        clientInitialized = elm327Client.begin(client, true, 10000);
        if (!clientInitialized)
        {
            log("Init elm client failed", "ELM327Manager");
            WiFi.reconnect();
            begin();
        }
    }
    initialized = elm327Client.initializeELM();

    log("Connection finalized correctly. Result: " + String(clientInitialized) + " Result init: " + String(initialized), "ELM327Manager");
}

void ELM327Manager::absBaroPressure()
{
    float absBaroPressure = elm327Client.absBaroPressure();

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Abs Baro Pressure is: " + String(absBaroPressure) + " kpa", "ELM327Manager.absBaroPressure");
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {

        elm327Client.printError();
    }
}

void ELM327Manager::fuelLevel()
{
    float fuelPercent = elm327Client.fuelLevel(); // Replace with actual call if different

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Fuel Level: " + String(fuelPercent) + "%", "ELM327Manager");
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {

        log("Error getting engine coolant temp", "ELM327Manager.fuelLevel");
        elm327Client.printError();
    }
}

void ELM327Manager::engineCoolantTemp()
{
    float tempValue = elm327Client.engineCoolantTemp(); // Replace with actual call if different

    if (elm327Client.nb_rx_state == ELM_SUCCESS)
    {
        log("Engine Coolant Temperature: " + String(tempValue) + "°C", "ELM327Manager.engineCoolantTemp");
    }
    else if (elm327Client.nb_rx_state != ELM_GETTING_MSG)
    {
        log("Error getting engine coolant temp", "ELM327Manager.engineCoolantTemp");
        elm327Client.printError();
    }
}
