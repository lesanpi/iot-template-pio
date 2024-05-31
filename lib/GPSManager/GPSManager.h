#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "Development.h"
#include <ArduinoJson.h>

struct GeolocationData
{
    float latitude;
    float longitude;
};

class GPSManager
{
public:
    GPSManager(int8_t rxPin, int8_t txPin, bool useMock = false);
    void begin();
    void loop();
    void logGPS();
    void restartDistanceTraveled();
    void restart();
    double getDistanceTraveled();
    bool isUpdated();
    static const uint32_t GPSBaud = 9600;
    bool isInitialized()
    {
        return initialized;
    }
    GeolocationData geolocation()
    {
        GeolocationData data;
        data.latitude = lastLatitud;
        data.longitude = lastLongitud;
        return data;
    }

private:
    void calculate();

    HardwareSerial *ss;
    bool useMock;
    bool initialized = false;
    double lastLatitud;
    double lastLongitud;
    int satellites = 0;

    double speedKpmh = 0;
    double hdop = 5;
    double distanceTraveled = 0;
    int8_t rxPin;
    int8_t txPin;
    bool updated = false;
    // The TinyGPS++ object
    TinyGPSPlus gps;
    /// Weights

    // HDOP weight factor (between 0 and 1).
    float hdopWeightFactor = 0.25;
    // Satellite weight factor (between 0 and 1).
    float satelliteWeightFactor = 0.5;
    // Maximum acceptable HDOP value.
    float hdopThreshold = 2.5;
    // Minimum number of satellites for a valid measurement.
    int minimumSatellites = 5;

    /// Minimum speed in Kmph to to detect movement in a vehicle
    float minimumSpeedKmph = 20.0f;
    float maximumSpeedKmph = 160.0f;

    void calculateSatellites();
    void calculateHdop();
    void calculateSpeed();

    bool isMoving()
    {
        /// Minimum speed to detect device is moving
        if (useMock)
            return true;

        bool isMoving = this->speedKpmh >= minimumSpeedKmph && this->speedKpmh <= maximumSpeedKmph;
        return isMoving;
    }
};

#endif