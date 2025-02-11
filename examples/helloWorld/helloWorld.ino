#include <PxServ.h>

// PxServ API Key (Enter your project API key here)
PxServ client("your_pxserv_api_key");

void setup()
{
    // Wi-Fi settings (Wi-Fi SSID and Password)
    Serial.begin(115200);
    PxServ::connectWifi("your_wifi_ssid", "your_wifi_password");
}

void loop()
{
    // Add data
    PxServ::Callback setResult = client.setData("msg", "value"); // Add "value" to the "msg" key
    Serial.print("Set Result -> Status: ");
    Serial.print(setResult.status);
    Serial.print(" | Message: ");
    Serial.print(setResult.message);
    Serial.print(" | Data: ");
    Serial.println(setResult.data);

    delay(2000); // Wait for two seconds

    // Get data
    PxServ::Callback getResult = client.getData("msg"); // Get the value for the "msg" key
    Serial.print("Get Result -> Status: ");
    Serial.print(getResult.status);
    Serial.print(" | Message: ");
    Serial.print(getResult.message);
    Serial.print(" | Data: ");
    Serial.println(getResult.data);

    delay(2000); // Wait for two seconds

    // Remove data
    PxServ::Callback removeResult = client.removeData("msg"); // Remove the "msg" key
    Serial.print("Remove Result -> Status: ");
    Serial.print(removeResult.status);
    Serial.print(" | Message: ");
    Serial.print(removeResult.message);
    Serial.print(" | Data: ");
    Serial.println(removeResult.data);

    delay(2000); // Wait for two seconds
}