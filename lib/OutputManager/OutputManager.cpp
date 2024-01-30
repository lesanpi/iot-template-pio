#include "OutputManager.h"

void OutputManager::setup()
{
    pinMode(this->greenPin, OUTPUT);
    pinMode(this->redPin, OUTPUT);
    pinMode(this->bluePin, OUTPUT);
}

void OutputManager::updateLEDs()
{
    switch (deviceState)
    {
    case DeviceState::NotConfigured:
        // LED pattern for NotConfigured state (e.g., blink both LEDs together)
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, LOW);
        break;
    case DeviceState::Running:
        // LED pattern for Running state (e.g., steady green LED)
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, LOW);
        break;
    case DeviceState::Updating:
        // LED pattern for Updating state (e.g., fast blinking blue LED)
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, HIGH);
        break;
    }
}