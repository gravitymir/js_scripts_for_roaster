#include <ArduinoJson.h>
// https://github.com/esp8266/Arduino/issues/2281
//  #define FS_NO_GLOBALS
//  #include "SPIFFS.h"

String galeryFolderPath = "/gallery";
String logoFolderPath = "/logo";
String rosterKitchenFolder = "/roster_k";
#include "display.h"
#include "auxilary.h"
#include "ntp.h"
#include "sdcard.h"
#include "roster.h"
#include "telegram.h"

#include "mywifi.h"
#include "weather.h"
#include "auxilaryLast.h"

#define LOOP_DELAY_MS 5000

void setup()
{
    Serial.begin(SERIAL_BAUD);
    display.begin();
    display.setTextColor(0, 7);
    display.setRotation(2);
    display.clearDisplay();
    display.sdCardInit();
    printLS();

    connectToWIFI();
    setTimeFromNTP();
    // makeRoster();

    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    // display.setDisplayMode(INKPLATE_3BIT); // INKPLATE_1BIT

    // display.drawImage("/logo/0.jpg", 0, 0, 1, 0);
    // display.display();
    // display.display();
    // delay(10000);
    // display.clearDisplay();

    // getWeather("52.564", "-8.79");
    //  ESP.restart();
    //  https://api.openweathermap.org/data/2.5/forecast?lat=52.564&lon=-8.79&&lang=en&units=metric&appid=f18eb09bf809871d1b5992f7b4eafdda
    // showTime();
    makeRoster();
    // playGallery();
    // delay(5000);
    // playGallery();
    // delay(5000);
    // playGallery();
    // delay(5000);
    // playGallery();
    // delay(5000);
    // display.clearDisplay();
    // getWeather("52.5612", "-8.8155");
}
void loop()
{
    checkTime();
    // showTime();
    // saveToPath("https://api.telegram.org/file/bot7503204474:AAFRb8OeB0uUHpAugCbiDwJEDppYJvdwKD4/documents/file_49.jpg", "/galery/10.jpg");
    // display.drawJpegFromWeb("https://api.telegram.org/file/bot7503204474:AAFRb8OeB0uUHpAugCbiDwJEDppYJvdwKD4/documents/file_49.jpg", 0, 0, 1, 0);
    // delay(3000);
    // playGallery();
    // delay(3000);
    // delay(30000);
    // display.drawJpegFromSd("/logo/10.jpeg", 0, 0, 1, 0);
    // display.display();
    // delay(3000);
    // telegramUpdate();
    
    //timeToSleep(0, 0, 60);
    //  display.clearDisplay();
    //  display.drawImage("/galery/1.jpeg", 0, 0, 1, 0);
    //  display.display();
    //  display.clearDisplay();
    //  display.drawImage("/galery/0.jpeg", 0, 0, 1, 0);
    //  display.display();
    //  display.drawImage(String(buffer), 0, 0, 1, 0);
    //  display.clearDisplay();
    //  display.drawImage("/logo/0.jpeg", 0, 0, 1, 0);
    //  display.display();
    // delay(5000);
    // makeRoster();
    // delay(60000);

    // timeToSleep(0, 0, 60);

    // if (millis() - MAKE_ROASTER_LAST_TIME > MAKE_ROASTER_DELAY_MS)
    // {
    //     Serial.println("makeRoster();");

    //     makeRoster();
    //     // display.display();
    //     // display.rtcGetRtcData();
    //     // Serial.print("Hour: ");
    //     // Serial.println(display.rtcGetHour());
    //     MAKE_ROASTER_LAST_TIME = millis();
    // }

    // delay(LOOP_DELAY_MS);
    // delay(DELAY_MS);

    // switch (display.rtcGetHour())
    // {
    // case 23:
    //     deepSleep(10);
    //     break;

    // default:
    //     deepSleep(3);
    //     break;
    // }
    // delay(LOOP_DELAY_MS);
}
