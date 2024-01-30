
#include "InputManager.h"
#include "Development.h"

InputManager::InputManager(int resetButtonPin, int resetTimeMax)
{
    this->resetButtonPin = resetButtonPin;
    this->resetTimeMax = resetTimeMax;
}

void InputManager::setup()
{
    pinMode(this->resetButtonPin, INPUT);
    this->resetButtonState = LOW;
    this->initialResetTime = 0;
}

void InputManager::loop()
{
    this->resetButtonState = digitalRead(resetButtonPin);

    // The button is presssed
    if (resetButtonState == HIGH)
    {
        // The time passed is bigger than the reset time
        if (millis() - initialResetTime > resetTimeMax)
        {
            log("🚨 Reset action detected", "InputManager.loop()");
            this->mustRestart = true;
        }
    }
    else
    {
        // The button is not pressed
        // Restart initial time
        this->initialResetTime = millis();
    }
}

bool InputManager::mustRestartDevice()
{
    return this->mustRestart;
}

void InputManager::restarted()
{
    this->resetButtonState = LOW;
    this->initialResetTime = 0;
    this->mustRestart = false;
}
