#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H
#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPSManager
{
public:
    GPSManager(int8_t rxPin, int8_t txPin, bool useMock = false);
    void begin();
    void loop();
    void logGPS();
    void restartDistanceTraveled();
    double getDistanceTraveled();
    bool isUpdated();
    static const uint32_t GPSBaud = 9600;

private:
    void calculate();

    HardwareSerial *ss;
    bool useMock;
    bool initialized;
    double lastLatitud;
    double lastLongitud;
    double distanceTraveled = 0;
    int8_t rxPin;
    int8_t txPin;
    bool updated = false;
    // The TinyGPS++ object
    TinyGPSPlus gps;
};

#endif