#include "WiFi.h"
#include "HTTPClient.h"

#define WIFI_SSID "PUB WIFI"
#define WIFI_PASSWORD "Abcd12342022"
// #define WIFI_SSID "eir97444542"
// #define WIFI_PASSWORD "W8GHRb99nB"
// #define WIFI_SSID "home"
// #define WIFI_PASSWORD "21212122"

void connectToWIFI();
void connectToWIFI()
{
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // WiFi.reconnect();
    // WiFi.mode(WIFI_OFF);
    //  bool sleep = WiFi.getSleep();
    //  WiFi.setSleep(false);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {

        Serial.printf("\rWiFi status: %d", WiFi.status());
        Serial.printf("\rConnecting to WIFI: %d", i);
        delay(100);
        i++;
    }
    Serial.print("\nWiFi IP address: ");
    Serial.println(WiFi.localIP());
};
