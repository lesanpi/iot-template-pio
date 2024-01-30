#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H
#include <Arduino.h>
#include "DeviceState.h"

/// @brief  Simple Output manager that controls 3 leds, then it will become into a controller for a RGB LED
class OutputManager
{
public:
    OutputManager(int redPin, int greenPin, int bluePin) : greenPin(redPin), redPin(greenPin), bluePin(bluePin){};

    void setup();

    void setState(DeviceState newState)
    {
        this->deviceState = newState;
        updateLEDs();
    }

    DeviceState getState()
    {
        return deviceState;
    }

private:
    const int redPin;
    const int greenPin;
    const int bluePin;
    DeviceState deviceState = DeviceState::NotConfigured;

    void updateLEDs();

    void setGreenPin(bool state)
    {
        digitalWrite(greenPin, state ? HIGH : LOW);
    }

    void setRedPin(bool state)
    {
        digitalWrite(redPin, state ? HIGH : LOW);
    }

    void setBluePin(bool state)
    {
        digitalWrite(bluePin, state ? HIGH : LOW);
    }
};

#endif