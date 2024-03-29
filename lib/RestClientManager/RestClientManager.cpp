#include <Arduino.h>
#include "Development.h"
#include <WiFi.h>
#include <HTTPClient.h>

enum HttpMethod
{
    GET,
    POST,
    PUT,
    PATCH,
    /// HttpClient not contains method delete
    DELETE
};

class HttpResponse
{
public:
    int statusCode;
    String body;

    HttpResponse(int code, const String &response)
    {
        statusCode = code;
        body = response;
    }
};

class RestClientManager
{
public:
    RestClientManager(WiFiClient &client)
    {
        this->client = &client;
    }

    RestClientManager(WiFiClient &client, HTTPClient *http)
    {
        this->client = &client;
        this->http = http;
    }

    void setWifiClient(WiFiClient &client)
    {
        this->client = &client;
    }

    WiFiClient &getWifiClient()
    {
        return *this->client;
    }

    void addHeader(const String &name, const String &value, bool first = false, bool replace = true)
    {
        http->addHeader(name, value);
    }

    HttpResponse request(String url, HttpMethod method, String data = "")
    {

        http->begin(*client, url);
        int httpStatusCode = -1;
        switch (method)
        {
        case GET:
            httpStatusCode = http->GET();
            break;
        case POST:
            httpStatusCode = http->POST(data);
            break;
        case PUT:
            httpStatusCode = http->PUT(data);
            break;
        case PATCH:
            httpStatusCode = http->PATCH(data);
            break;
        case DELETE:
            log("DELETE is not developed in HTTPClient library", "RestClientManager.request()");
            break;
        }

        if (httpStatusCode > 0)
        {
            String payload = http->getString();
            http->end();
            return HttpResponse(httpStatusCode, payload);
        }
        else
        {
            log("Error al realizar la petición: " + String(httpStatusCode), "RestClientManager.request()");
            http->end();
            return HttpResponse(500, "");
        }
    }

private:
    WiFiClient *client;
    HTTPClient *http;
};
