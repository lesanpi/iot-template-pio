
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <Arduino.h>

class InputManager
{
public:
    InputManager(int resetButtonPin, int resetTimeMax);
    void setup();
    void loop();
    bool mustRestartDevice();
    void restarted();

private:
    int resetButtonPin;
    int resetTimeMax;
    int resetButtonState = LOW;
    unsigned long initialResetTime = 0;
    bool mustRestart = false;
};

#endif