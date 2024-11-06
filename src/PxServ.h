#ifndef PxServ_h
#define PxServ_h

#include <Arduino_JSON.h>
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

class PxServ
{
public:
    struct Callback
    {
        int status;
        String message;
        String data;
    };

    PxServ(String apiKey);

private:
    String _apiKey;
};

#endif