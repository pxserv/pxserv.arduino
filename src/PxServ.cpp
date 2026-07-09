#include "PxServ.h"

PxServ::PxServ(String apiKey)
{
    _apiKey = apiKey;
}

void PxServ::connectWifi(String ssid, String password)
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("[PxServ] Connecting to WiFi...");
    }

    Serial.println("[PxServ] Connected to WiFi");
}

void PxServ::setDeviceFirmwareVersion(String version)
{
    _otaFirmwareVersion = version;
}
String PxServ::getDeviceFirmwareVersion()
{
    return _otaFirmwareVersion;
}
void PxServ::checkOtaFirmware()
{
    const unsigned long OTA_CHECK_INTERVAL = 10000;

    if (millis() - _lastOtaCheck < OTA_CHECK_INTERVAL)
    {
        return;
    }

    _lastOtaCheck = millis();

    WiFiClientSecure *client = new WiFiClientSecure;

    if (client)
    {
        client->setInsecure();

        HTTPClient https;

        if (https.begin(*client, "https://api.pxserv.net/ota/check"))
        {
            https.addHeader("Content-Type", "application/json");
            https.addHeader("apikey", _apiKey);

            int httpCode = https.POST("{}");

            if (httpCode > 0)
            {
                String payload = https.getString();

                DynamicJsonDocument result(payload.length() + 256);
                DeserializationError error = deserializeJson(result, payload);

                if (!error && result.is<JsonObject>())
                {
                    int status = result["status"].as<int>();

                    if (status == 200)
                    {
                        String lastVersion = result["data"]["version"].as<String>();
                        String fileUrl = result["data"]["fileUrl"].as<String>();

                        if (_otaFirmwareVersion != lastVersion)
                        {
                            Serial.println("--------------------------------------------------");
                            Serial.println("[PxServ] Updating OTA Firmware ...");
                            Serial.println("[PxServ] Update Version : " + lastVersion);
                            Serial.println("[PxServ] Current Version : " + _otaFirmwareVersion);
                            Serial.println("--------------------------------------------------");

                            https.end();

                            WiFiClientSecure otaClient;
                            otaClient.setInsecure();

#if defined(ESP32)
                            t_httpUpdate_return ret = httpUpdate.update(otaClient, fileUrl);
#elif defined(ESP8266)
                            otaClient.setBufferSizes(1024, 1024);
                            t_httpUpdate_return ret = ESPhttpUpdate.update(otaClient, fileUrl);
#endif

                            switch (ret)
                            {
                            case HTTP_UPDATE_FAILED:
#if defined(ESP32)

                                Serial.println("[PxServ] OTA ERROR: " + httpUpdate.getLastErrorString());
#elif defined(ESP8266)
                                Serial.println("[PxServ] OTA ERROR: " + ESPhttpUpdate.getLastErrorString());

#endif

                                break;

                            case HTTP_UPDATE_NO_UPDATES:
                                break;

                            case HTTP_UPDATE_OK:
                                // restart
                                break;
                            }

                            delete client;
                            return;
                        }
                    }
                }
            }

            https.end();
        }
    }

    delete client;
}

PxServ::Callback PxServ::setData(String key, String value)
{
    WiFiClientSecure *client = new WiFiClientSecure;
    Callback callback;

    callback.status = -1;
    callback.message = "failed to send request";
    callback.data = "";

    if (client)
    {
        client->setInsecure();

        HTTPClient https;

        if (https.begin(*client, "https://api.pxserv.net/database/setData"))
        {
            https.addHeader("Content-Type", "application/json");
            https.addHeader("apikey", _apiKey);

            StaticJsonDocument<512> body;
            body["key"] = key;
            body["value"] = value;
            String bodyStr;
            serializeJson(body, bodyStr);

            int httpCode = https.POST(bodyStr);
            if (httpCode > 0)
            {
                String payload = https.getString();
                DynamicJsonDocument result(payload.length() + 256);
                DeserializationError error = deserializeJson(result, payload);

                if (error || !result.is<JsonObject>())
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    callback.status = result["status"].as<int>();
                    callback.message = result["message"].as<String>();
                }

                https.end();
            }
        }
    }

    delete client;
    return callback;
}

PxServ::Callback PxServ::toggleData(String key)
{
    WiFiClientSecure *client = new WiFiClientSecure;
    Callback callback;

    callback.status = -1;
    callback.message = "failed to send request";
    callback.data = "";

    if (client)
    {
        client->setInsecure();

        HTTPClient https;

        if (https.begin(*client, "https://api.pxserv.net/database/toggleData"))
        {
            https.addHeader("Content-Type", "application/json");
            https.addHeader("apikey", _apiKey);

            StaticJsonDocument<256> body;
            body["key"] = key;
            String bodyStr;
            serializeJson(body, bodyStr);

            int httpCode = https.POST(bodyStr);
            if (httpCode > 0)
            {
                String payload = https.getString();
                DynamicJsonDocument result(payload.length() + 256);
                DeserializationError error = deserializeJson(result, payload);

                if (error || !result.is<JsonObject>())
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    callback.status = result["status"].as<int>();
                    callback.message = result["message"].as<String>();
                }

                https.end();
            }
        }
    }

    delete client;
    return callback;
}

PxServ::Callback PxServ::getData(String key)
{
    WiFiClientSecure *client = new WiFiClientSecure;
    Callback callback;

    callback.status = -1;
    callback.message = "failed to send request";
    callback.data = "";

    if (client)
    {
        client->setInsecure();

        HTTPClient https;

        if (https.begin(*client, "https://api.pxserv.net/database/getData"))
        {
            https.addHeader("Content-Type", "application/json");
            https.addHeader("apikey", _apiKey);

            StaticJsonDocument<256> body;
            body["key"] = key;
            String bodyStr;
            serializeJson(body, bodyStr);

            int httpCode = https.POST(bodyStr);
            if (httpCode > 0)
            {
                String payload = https.getString();
                DynamicJsonDocument result(payload.length() + 256);
                DeserializationError error = deserializeJson(result, payload);

                if (error || !result.is<JsonObject>())
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    int status = result["status"].as<int>();
                    String message = result["message"].as<String>();

                    if (status == 200)
                    {
                        callback.data = result["data"]["value"].as<String>();
                    }

                    callback.status = status;
                    callback.message = message;
                }

                https.end();
            }
        }
    }

    delete client;
    return callback;
}

PxServ::Callback PxServ::removeData(String key)
{
    WiFiClientSecure *client = new WiFiClientSecure;
    Callback callback;

    callback.status = -1;
    callback.message = "failed to send request";
    callback.data = "";

    if (client)
    {
        client->setInsecure();

        HTTPClient https;

        if (https.begin(*client, "https://api.pxserv.net/database/removeData"))
        {
            https.addHeader("Content-Type", "application/json");
            https.addHeader("apikey", _apiKey);

            StaticJsonDocument<256> body;
            body["key"] = key;
            String bodyStr;
            serializeJson(body, bodyStr);

            int httpCode = https.POST(bodyStr);
            if (httpCode > 0)
            {
                String payload = https.getString();
                DynamicJsonDocument result(payload.length() + 256);
                DeserializationError error = deserializeJson(result, payload);

                if (error || !result.is<JsonObject>())
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    callback.status = result["status"].as<int>();
                    callback.message = result["message"].as<String>();
                }

                https.end();
            }
        }
    }

    delete client;
    return callback;
}
