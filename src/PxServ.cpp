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

            JSONVar body;
            body["key"] = key;
            body["value"] = value;

            int httpCode = https.POST(JSON.stringify(body));
            if (httpCode > 0)
            {
                String payload = https.getString();
                JSONVar result = JSON.parse(payload);

                if (JSON.typeof(result) != "object")
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    int status = result["status"];
                    String message = result["message"];

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

            JSONVar body;
            body["key"] = key;

            int httpCode = https.POST(JSON.stringify(body));
            if (httpCode > 0)
            {
                String payload = https.getString();
                JSONVar result = JSON.parse(payload);

                if (JSON.typeof(result) != "object")
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    int status = result["status"];
                    String message = result["message"];

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

            JSONVar body;
            body["key"] = key;

            int httpCode = https.POST(JSON.stringify(body));
            if (httpCode > 0)
            {
                String payload = https.getString();
                JSONVar result = JSON.parse(payload);

                if (JSON.typeof(result) != "object")
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    int status = result["status"];
                    String message = result["message"];

                    if (status == 200)
                    {
                        String value = result["data"]["value"];
                        callback.data = value;
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

            JSONVar body;
            body["key"] = key;

            int httpCode = https.POST(JSON.stringify(body));
            if (httpCode > 0)
            {
                String payload = https.getString();
                JSONVar result = JSON.parse(payload);

                if (JSON.typeof(result) != "object")
                {
                    callback.status = 400;
                    callback.message = "Response format not appropriate";
                }
                else
                {
                    int status = result["status"];
                    String message = result["message"];

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
