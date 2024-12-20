#include "GPSManager.h"

GPSManager::GPSManager(int8_t rxPin, int8_t txPin, bool useMock)
{
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->useMock = useMock;
    ss = new HardwareSerial(2);
}

void GPSManager::loop()
{
    // log("Executing loop, using mock: " + String(useMock), "GPSManager.loop()");
    calculateSatellites();
    calculateHdop();
    calculateSpeed();
    if (useMock)
    {
        // A sample NMEA stream.
        const char *gpsStream =
            "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
            "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
            "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
            "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
            "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
            "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
        while (*gpsStream)
        {
            if (gps.encode(*gpsStream++))
            {

                logGPS();
                delay(1000);
                calculate();
            }
        }
        return;
    }

    while (ss->available() > 0)
    {
        // Serial.print(String(ss->read()));
        if (gps.encode(ss->read()))
        {
            logGPS();
            calculate();
        }
    }

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        log(F("No GPS detected: check wiring."), "GPSManager.loop()");
        while (true)
            ;
    }
}

void GPSManager::begin()
{
    log("🛰️ Starting GPS Manager...", "GPSManager.begin()");
    ss->begin(GPSBaud, SERIAL_8N1, rxPin, txPin, false);
    log(TinyGPSPlus::libraryVersion(), "GPSManager.begin()");
}

void GPSManager::restart()
{
    this->distanceTraveled = 0;
    this->updated = false;
    this->initialized = false;
    this->satellites = 0;
    this->hdop = 5;
}

void GPSManager::restartDistanceTraveled()
{
    this->distanceTraveled = 0;
    this->updated = false;
}

double GPSManager::getDistanceTraveled()
{
    return this->distanceTraveled;
}

void GPSManager::calculateSatellites()
{
    int numSatellites = gps.satellites.value();
    if (gps.satellites.isValid())
    {
        if (this->satellites != numSatellites)
        {

            log("🛰️ GPS satellites number: " + String(numSatellites), "GPSManager.calculateSatellites()");
            this->satellites = numSatellites;
        }
    }
}

void GPSManager::calculateHdop()
{
    int hdopCalculated = gps.hdop.hdop();
    if (gps.hdop.isValid())
    {
        if (this->hdop != hdopCalculated)
        {

            log("🛰️ GPS HDOP: " + String(hdopCalculated), "GPSManager.calculateHdop()");
            this->hdop = hdopCalculated;
        }
    }
}
void GPSManager::calculateSpeed()
{
    double kmphUpdated = gps.speed.kmph();
    if (gps.speed.isValid())
    {
        if (this->speedKpmh != kmphUpdated)
        {

            this->speedKpmh = kmphUpdated;
            log("🏎️ GPS Speed: " + String(kmphUpdated) + ". 👟 Is moving? " + String(isMoving() ? "✅" : "❌"), "GPSManager.calculateSpeed()");
        }
    }
}

void GPSManager::calculate()
{

    if (gps.location.isValid())
    {
        double lat = gps.location.lat();
        double lon = gps.location.lng();
        changeConnected(true);

        /// The following code is according your project

        if (!this->initialized)
        {
            this->lastLatitud = lat;
            this->lastLongitud = lon;
            this->initialized = true;
            log("🗺️✅ Initial coordinates saved: " + String(lat, 6) + ", " + String(lon, 6), "GPSManager.calculate()");
            return;
        }

        // Calculate the weighted distance according the last sample
        double distanceBetweenMeters = TinyGPSPlus::distanceBetween(this->lastLatitud, this->lastLongitud, lat, lon);

        /// Device is not moving
        if (!isMoving())
            return;

        /// Verify minimum conditions
        if (this->hdop <= hdopThreshold && this->satellites >= minimumSatellites)
        {
            // Distance calculation with HDOP and number of satellites weighting
            distanceBetweenMeters *= (1.0 - hdopWeightFactor - satelliteWeightFactor) +
                                     (hdopWeightFactor * (hdopThreshold - this->hdop) / hdopThreshold) +
                                     (satelliteWeightFactor * (this->satellites - minimumSatellites) / (12.0 - minimumSatellites));
        }
        else
        {
            // Invalid conditions
            return;
        }

        // Save the coordinates of the distance is significantly (greater than 25 meters)
        if (distanceBetweenMeters >= 35)
        {
            log("🚗 Distance between: " + String(distanceBetweenMeters, 8), "GPSManager.calculate()");
            this->lastLatitud = lat;
            this->lastLongitud = lon;
            this->distanceTraveled = this->distanceTraveled + (distanceBetweenMeters / 1000);
            this->updated = true;
            log("✅ Coordinates saved: " + String(lat, 6) + ", " + String(lon, 6), "GPSManager.calculate()");
        }
    }
    else
    {
        changeConnected(false);
    }
}

bool GPSManager::isUpdated()
{
    return this->updated;
}

void GPSManager::logGPS()
{
    // log("HDOP: " + String(gps.hdop.hdop()), "GPSManager.logGPS(hdop)");

    if (gps.location.isValid())
    {
        // log("🗺️ Location: " + String(gps.location.lat(), 8) + "," + String(gps.location.lng(), 8), "GPSManager.logGPS()");
    }
    else
    {
        log("❌ Invalid location", "GPSManager.logGPS()");
    }
}