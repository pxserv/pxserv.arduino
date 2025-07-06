#ifndef PxServ_h
#define PxServ_h

#include <Arduino_JSON.h>
#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#else
#error "PxServ Arduino Library only supports ESP32 and ESP8266 boards.\nPxServ Arduino Kütüphanesi sadece ESP32 ve ESP8266 kartlarında çalışır."
#endif

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
    static void connectWifi(String ssid, String password);
    Callback setData(String key, String value);
    Callback toggleData(String key);
    Callback getData(String key);
    Callback removeData(String key);

private:
    String _apiKey;
};

#endif