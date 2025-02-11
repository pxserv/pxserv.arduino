#include <PxServ.h>

// PxServ API Anahtarı (Proje API anahtarınızı buraya girin)
PxServ client("pxserv_api_key");

void setup()
{
    // Wi-Fi ayarları (Wi-Fi SSID ve Şifre)
    Serial.begin(115200);
    PxServ::connectWifi("wifi_ssid", "wifi_sifre");
}

void loop()
{
    // Veri Yazdırma
    PxServ::Callback setResult = client.setData("msg", "value"); // "msg" anahtarına "value" değerini ekle
    Serial.print("Veri Ekleme Sonucu -> Durum: ");
    Serial.print(setResult.status);
    Serial.print(" | Mesaj: ");
    Serial.print(setResult.message);
    Serial.print(" | Veri: ");
    Serial.println(setResult.data);

    delay(2000); // İki saniye bekle

    // Veri Okuma
    PxServ::Callback getResult = client.getData("msg"); // "msg" anahtarının değerini getir
    Serial.print("Veri Okuma Sonucu -> Durum: ");
    Serial.print(getResult.status);
    Serial.print(" | Mesaj: ");
    Serial.print(getResult.message);
    Serial.print(" | Veri: ");
    Serial.println(getResult.data);

    delay(2000); // İki saniye bekle

    // Veri Kaldırma
    PxServ::Callback removeResult = client.removeData("msg"); // "msg" anahtarını kaldır
    Serial.print("Kaldırma Sonucu -> Durum: ");
    Serial.print(removeResult.status);
    Serial.print(" | Mesaj: ");
    Serial.print(removeResult.message);
    Serial.print(" | Veri: ");
    Serial.println(removeResult.data);

    delay(2000); // İki saniye bekle
}