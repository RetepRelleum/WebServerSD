#include <WiFi.h>
#include <WiFiServer.h>
#include <WebServer.h>
#include <Arduino.h>
#include <SD.h>
#include <Debug.h>
#include "secret.h"



#ifdef __ESP_ARDU
WiFiServer server(8080);
WebServer webServer(server,"/home/retep/projects/WebServ/WebRoot");
#else
WiFiServer server(80);
WebServer webServer(server,"/webRoot");
#endif


String translateEncryptionType(wifi_auth_mode_t encryptionType)
{
    switch (encryptionType)
    {
    case (WIFI_AUTH_OPEN):
        return "Open";
    case (WIFI_AUTH_WEP):
        return "WEP";
    case (WIFI_AUTH_WPA_PSK):
        return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
        return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
        return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
        return "WPA2_ENTERPRISE";
    default:
        return "";
    }
}
fs::File f;
fs::File fo;
void scanNetworks() 
{
    int numberOfNetworks = WiFi.scanNetworks();
    debug_println(String(" Number of networks found:   ") + String(numberOfNetworks));

    for (int i = 0; i < numberOfNetworks; i++)
    {
        debug_println(String(" Network name:     ") + String(WiFi.SSID(i).c_str()));
        debug_println(String(" Signal strength:  ") + String(WiFi.RSSI(i)));
        debug_println(String(" MAC address:      ") + WiFi.BSSIDstr(i));
        debug_println(String(" Encryption type:  ") + translateEncryptionType((wifi_auth_mode_t)WiFi.encryptionType(i)));
        debug_println(String(" -----------------------"));
    }
}

void connectToNetwork()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        debug_println(String(" Establishing connection to WiFi..."));
    }
    debug_println(String(" Connected to network"));
}

void setup()
{
    Serial.begin(115200);
    scanNetworks();
    connectToNetwork();
    debug_println(String(" Mac Address ") + WiFi.macAddress());
    debug_println(String(" IP Address  ") + WiFi.localIP().toString());
    debug_println(String(" Gateway IP  ") + WiFi.gatewayIP().toString());

    if (!SD.begin(4))
    {
        Serial.println("Card Mount Failed");
        return;
    }

    webServer.begin();

}


void loop()
{
    webServer.loop();
}



