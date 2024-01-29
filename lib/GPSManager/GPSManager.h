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
    double getDistanceTraveledFromLastUpdate();
    static const uint32_t GPSBaud = 9600;

private:
    HardwareSerial *ss;
    bool useMock;
    bool initialized;
    double lastLatitud;
    double lastLongitud;
    int8_t rxPin;
    int8_t txPin;
    void calculate();
    // The TinyGPS++ object
    TinyGPSPlus gps;
};

#endif