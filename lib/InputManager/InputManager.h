
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <Arduino.h>

class InputManager
{
public:
    InputManager(int resetButtonPin, int resetTimeMax);
    void setup();
    bool mustRestartDevice();
    void restarted();

private:
    void loop();
    int resetButtonPin;
    int resetTimeMax;
    int resetButtonState = LOW;
    unsigned long initialResetTime = 0;
    bool mustRestart = false;
};

#endif