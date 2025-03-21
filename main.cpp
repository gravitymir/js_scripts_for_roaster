#include <Arduino.h>
#include "Inkplate.h"

#include "WiFi.h"
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include "HTTPClient.h"
#include <uri/UriBraces.h>

#include "Fonts/JetBrainsExtraBold_40.h"
#include "HTML/htmlCode.h"

#include <string>
#include <iostream>
#include "time.h"
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>

#define DELAY_MS 5000
#define SERIAL_BAUD 115200

#define NTP_SERVER "pool.ntp.org"

#define WIFI_SSID "home"
#define WIFI_PASSWORD "21212122"

#define AP_SSID "Inkplate"
#define AP_PASSWORD "21212122"

// #define BOT_TOKEN "" // enebra
#define BOT_TOKEN "" // patcollinsbar

// mean time between scan messages millis
#define BOT_MTBS 10000
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

Inkplate display(INKPLATE_3BIT);
// const char ssid[] = "eir97444542";    // Your WiFi SSID
// const char *password = "W8GHRb99nB"; // Your WiFi password
// const char ssid[] = "home";        // Your WiFi SSID
// const char *password = "21212122"; // Your WiFi password
// put function declarations here:
void displayCurrentAction(String text);
void drawHorizontalLine(int y, int color);
void drawVerticalLine(int x, int color);
void makeHorizontalLines();
void makeVerticalLines();
void printLocalTime();
void printTime(uint8_t _hour, uint8_t _minutes, uint8_t _seconds, uint8_t _day, uint8_t _weekday, uint8_t _month,
               uint16_t _year);
void print2Digits(uint8_t _d);
void handleNewMessages(int numNewMessages);
void appendString(char *destination, String appenStr);

const long gmtOffset_sec = -3600;
const int daylightOffset_sec = 3600;

unsigned long bot_lasttime; // last time messages' scan has been done

// Set clock
uint8_t hour = 12;
uint8_t minutes = 50;
uint8_t seconds = 30;

// Set date and weekday (NOTE: In weekdays 0 means Sunday, 1 means Monday, ...)
uint8_t weekday = 4;
uint8_t day = 11;
uint8_t month = 11;
uint8_t year = 21;

void setup()
{
    // display.setDisplayMode(INKPLATE_3BIT); // INKPLATE_1BIT
    display.begin();
    display.setFont(&jet40pt7b);
    display.setTextSize(3);
    display.setTextColor(0, 7);
    display.setRotation(2);
    display.clearDisplay();

    Serial.begin(SERIAL_BAUD);
    // Serial.println();
    // Serial.print("Connecting to Wifi SSID ");
    // Serial.print(WIFI_SSID);

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

    Serial.print("\nWiFi connected. IP address: ");
    Serial.println(WiFi.localIP());
    display.display();

    Serial.print("Retrieving time: ");
    configTime(0, 0, NTP_SERVER); // get UTC time via NTP
    time_t now = time(nullptr);
    while (now < 24 * 3600)
    {
        Serial.print(".");
        delay(100);
        now = time(nullptr);
    }

    // display.rtcBegin();
    //  if(!display.rtcChangeTimeFormat()){
    //      Serial.print("Time format is 24H");
    //  }else{
    //      Serial.print("Time format is 12H");
    //  }
    Serial.println(now);

    // makeHorizontalLines();
    // makeVerticalLines();
    // display.display();
    // delay(5000);

    // Start the web server
    // updatePaper();
    //    int temperature = display.readTemperature();
    //    display.print(temperature, DEC);
    //    display.print('C');

    display.setCursor(100, 200);
    float voltage = display.readBattery();
    display.print(voltage, 2);
    display.print('V');
    display.display();

    //   display.setCursor(100, 600);

    //   display.print("Connecting to WiFi...");

    // WiFi.mode(WIFI_MODE_STA);
    // WiFi.begin(SSID, PASSWORD);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     // display.print(".");
    // }
    // configTime(gmtOffset_sec, daylightOffset_sec, NTP_SERVER);
    // printLocalTime();
    // // display.println("\nWiFi OK! Downloading...");

    // display.display();
    // delay(1000);
    // display.clearDisplay();
    // if (!display.drawImage("https://raw.githubusercontent.com/gravitymir/js_scripts_for_roaster/refs/heads/master/77.jpg", 0, 0, false, false))
    // {
    //     // If is something failed (wrong filename or wrong bitmap format), write error message on the screen.
    //     // REMEMBER! You can only use Windows Bitmap file with color depth of 1, 4, 8 or 24 bits with no compression!
    //     display.println("Image open error");
    //     display.display();
    // }
    // display.display();
    // Serial.println("77");
    // delay(200000);
}
void loop()
{

    if (millis() - bot_lasttime > BOT_MTBS)
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        Serial.println(numNewMessages);
        while (numNewMessages)
        {
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }

        display.clearDisplay();
        printLocalTime();
        display.display();

        bot_lasttime = millis();
    }
}

// void loop()
// {
//     // Example will demostrate funcionality one by one. You always first set everything in the frame buffer and
//     // afterwards you show it on the screen using display.display().

//     // Let'sstart by drawing pixel at x = 100 and y = 50 location
//     display.clearDisplay();        // Clear everytning that is inside frame buffer in ESP32
//     display.drawPixel(100, 50, 0); // Draw one black pixel at X = 100, Y = 50 position in 0 (BLACK) color
//     displayCurrentAction(
//         "Drawing a pixel"); // Function which writes small text at bottom left indicating what's currently done
//                             // NOTE: you do not need displayCurrentAction function to use Inkplate!
//     display.display();      // Send image to display. You need to call this one each time you want to transfer frame buffer
//                             // to the screen.
//     delay(DELAY_MS);        // Wait a little bit

//     // Now, let's draw some random pixels!
//     display.clearDisplay(); // Clear everything that is inside frame buffer in ESP32
//     for (int i = 0; i < 1000; i++)
//     { // Write 1000 random colored pixels at random locations
//         display.drawPixel(random(0, 1199), random(0, 824),
//                           random(0, 7)); // We are setting color of the pixels using numbers from 0 to 7,
//     } // where 0 mens black, 7 white and gray is in between
//     displayCurrentAction("Drawing 600 random pixels in random colors");
//     display.display(); // Write everything from frame buffer to screen
//     delay(DELAY_MS);   // Wait

//     // Draw two diagonal lines accros screen
//     display.clearDisplay();
//     display.drawLine(
//         0, 0, 1199, 824,
//         0);                               // All of those drawing fuctions originate from Adafruit GFX library, so maybe you are already familiar
//     display.drawLine(1199, 0, 0, 824, 0); // with those. Arguments are: start X, start Y, ending X, ending Y, color.
//     displayCurrentAction("Drawing two diagonal lines");
//     display.display();
//     delay(DELAY_MS);

//     // And again, let's draw some random lines on screen!
//     display.clearDisplay();
//     for (int i = 0; i < 100; i++)
//     {
//         display.drawLine(random(0, 1199), random(0, 824), random(0, 1199), random(0, 824), random(0, 7));
//     }
//     displayCurrentAction("Drawing 50 random lines in random colors");
//     display.display();
//     delay(DELAY_MS);

//     // Let's draw some random thick lines on screen!
//     display.clearDisplay();
//     for (int i = 0; i < 100; i++)
//     {
//         display.drawThickLine(random(0, 1199), random(0, 824), random(0, 1199), random(0, 824), random(0, 7),
//                               (float)random(1, 20));
//     }
//     displayCurrentAction("Drawing 50 random lines in random colors and thickness");
//     display.display();
//     delay(DELAY_MS);

//     // Let's draw some random gradient thick lines on screen!
//     display.clearDisplay();
//     for (int i = 0; i < 100; i++)
//     {
//         int startColor = random(0, 7);
//         int endColor = random(startColor, 7);
//         display.drawGradientLine(random(0, 1199), random(0, 824), random(0, 1199), random(0, 824), startColor, endColor,
//                                  (float)random(1, 20));
//     }
//     displayCurrentAction("Drawing 50 random gradient lines in random colors and thickness");
//     display.display();
//     delay(DELAY_MS);

//     // Now draw one horizontal...
//     display.clearDisplay();
//     display.drawFastHLine(100, 100, 600, 0); // Arguments are: starting X, starting Y, length, color
//     displayCurrentAction("Drawing one horizontal line");
//     display.display();
//     delay(DELAY_MS);

//     //... and one vertical line
//     display.clearDisplay();
//     display.drawFastVLine(100, 100, 600, 0); // Arguments are: starting X, starting Y, length, color
//     displayCurrentAction("Drawing one vertical line");
//     display.display();
//     delay(DELAY_MS);

//     // Now, let' make a grid using only horizontal and vertical lines in random colors!
//     display.clearDisplay();
//     for (int i = 0; i < 1200; i += 8)
//     {
//         display.drawFastVLine(i, 0, 825, (i / 8) & 0x0F);
//     }
//     for (int i = 0; i < 825; i += 4)
//     {
//         display.drawFastHLine(0, i, 1200, (i / 8) & 0x0F);
//     }
//     displayCurrentAction("Drawing a grid using horizontal and vertical lines in different colors");
//     display.display();
//     delay(DELAY_MS);

//     // Draw rectangle at X = 200, Y = 200 and size of 600x412 pixels
//     display.clearDisplay();
//     display.drawRect(200, 200, 600, 412, 0); // Arguments are: start X, start Y, size X, size Y, color
//     displayCurrentAction("Drawing rectangle");
//     display.display();
//     delay(DELAY_MS);

//     // Draw rectangles on random location, size 100x150 pixels in random color
//     display.clearDisplay();
//     for (int i = 0; i < 50; i++)
//     {
//         display.drawRect(random(0, 1199), random(0, 824), 100, 150, random(0, 7));
//     }
//     displayCurrentAction("Drawing many rectangles in random colors");
//     display.display();
//     delay(DELAY_MS);

//     // Draw filled black rectangle at X = 200, Y = 200, size of 600x412 pixels in gray color
//     display.clearDisplay();
//     display.fillRect(200, 200, 700, 500, 4); // Arguments are: start X, start Y, size X, size Y, color
//     displayCurrentAction("Drawing gray rectangle");
//     display.display();
//     delay(DELAY_MS);

//     // Draw filled random colored rectangles on random location, size of 30x30 pixels in radnom color
//     display.clearDisplay();
//     for (int i = 0; i < 50; i++)
//     {
//         display.fillRect(random(0, 1199), random(0, 824), 30, 30, random(0, 7));
//     }
//     displayCurrentAction("Drawing many filled rectangles randomly in random colors");
//     display.display();
//     delay(DELAY_MS);

//     // Draw circle at center of a screen with radius of 75 pixels
//     display.clearDisplay();
//     display.drawCircle(600, 412, 75, 0); // Arguments are: start X, start Y, radius, color
//     displayCurrentAction("Drawing a circle");
//     display.display();
//     delay(DELAY_MS);

//     // Draw some random colored circles at random location with radius of 25 pixels in random color
//     display.clearDisplay();
//     for (int i = 0; i < 40; i++)
//     {
//         display.drawCircle(random(0, 1199), random(0, 824), 25, random(0, 7));
//     }
//     displayCurrentAction("Drawing many circles randomly in random colors");
//     display.display();
//     delay(DELAY_MS);

//     // Draw black filled circle at center of a screen with radius of 75 pixels
//     display.clearDisplay();
//     display.fillCircle(600, 412, 75, 0); // Arguments are: start X, start Y, radius, color
//     displayCurrentAction("Drawing black-filled circle");
//     display.display();
//     delay(DELAY_MS);

//     // Draw some random colored filled circles at random location with radius of 15 pixels
//     display.clearDisplay();
//     for (int i = 0; i < 40; i++)
//     {
//         display.fillCircle(random(0, 1199), random(0, 824), 15, random(0, 7));
//     }
//     displayCurrentAction("Drawing many filled circles randomly in random colors");
//     display.display(); // To show stuff on screen, you always need to call display.display();
//     delay(DELAY_MS);

//     // Draw rounded rectangle at X = 200, Y = 200 and size of 600x412 pixels and radius of 10 pixels
//     display.clearDisplay();
//     display.drawRoundRect(200, 200, 700, 500, 10, 0); // Arguments are: start X, start Y, size X, size Y, radius, color
//     displayCurrentAction("Drawing rectangle with rounded edges");
//     display.display();
//     delay(DELAY_MS);

//     // Draw rounded rectangles on random location, size 100x150 pixels, radius of 5 pixels in radnom color
//     display.clearDisplay();
//     for (int i = 0; i < 50; i++)
//     {
//         display.drawRoundRect(random(0, 1199), random(0, 824), 100, 150, 5, random(0, 7));
//     }
//     displayCurrentAction("Drawing many rounded edges rectangles");
//     display.display();
//     delay(DELAY_MS);

//     // Draw filled random colored rectangle at X = 200, Y = 200, size of 600x412 pixels and radius of 10 pixels
//     display.clearDisplay();
//     display.fillRoundRect(200, 200, 700, 500, 10, 0); // Arguments are: start X, start Y, size X, size Y, radius, color
//     displayCurrentAction("Drawing filled rectangle with rounded edges");
//     display.display();
//     delay(DELAY_MS);

//     // Draw filled random colored rectangle on random location, size of 30x30 pixels, radius of 3 pixels in radnom color
//     display.clearDisplay();
//     for (int i = 0; i < 50; i++)
//     {
//         display.fillRoundRect(random(0, 1199), random(0, 824), 30, 30, 3, random(0, 7));
//     }
//     displayCurrentAction("Drawing many filled rectangle with rounded edges in random colors");
//     display.display();
//     delay(DELAY_MS);

//     // Draw simple triangle
//     display.clearDisplay();
//     display.drawTriangle(250, 600, 550, 600, 600, 100, 0); // Arguments are: X1, Y1, X2, Y2, X3, Y3, color
//     display.display();
//     delay(DELAY_MS);

//     // Draw filled triangle inside simple triangle (so no display.clearDisplay() this time)
//     display.fillTriangle(412, 350, 500, 350, 600, 150, 0); // Arguments are: X1, Y1, X2, Y2, X3, Y3, color
//     displayCurrentAction("Drawing filled triangle inside exsisting one");
//     display.display();
//     delay(DELAY_MS);

//     // Write some text on screen with different sizes and color
//     display.clearDisplay();
//     for (int i = 1; i < 3; i++)
//     {
//         display.setTextColor(i);
//         display.setTextSize(
//             i + 1);                          // textSize parameter starts at 0 and goes up to 10 (larger won't fit Inkplate 10 screen)
//         display.setCursor(200, (i * i * 8)); // setCursor works as same as on LCD displays - sets "the cursor" at the
//                                              // place you want to write someting next
//         display.print("INKPLATE10!");        // The actual text you want to show on e-paper as String
//     }
//     displayCurrentAction("Text in different sizes and shadings");
//     display.display(); // To show stuff on screen, you always need to call display.display();
//     delay(DELAY_MS);

//     // Write same text on different location, but now invert colors (text is white, text background is black)
//     display.setTextColor(7, 0); // First argument is text color, while second argument is background color. In
//                                 // greyscale,
//     for (int i = 0; i < 3; i++)
//     { // you are able to choose from 8 different colors (0-7)
//         display.setTextSize(i + 1);
//         display.setCursor(200, 412 + (i * i * 8));
//         display.print("INKPLATE10!");
//     }
//     display.display();
//     delay(DELAY_MS);

//     // Draws an elipse with x radius, y radius, center x, center y and color
//     display.clearDisplay();
//     display.drawElipse(100, 412, 600, 412, 0);
//     displayCurrentAction("Drawing an elipse");
//     display.display();

//     delay(DELAY_MS);

//     // Fills an elipse with x radius, y radius, center x, center y and color
//     display.clearDisplay();
//     display.fillElipse(100, 412, 600, 412, 0);
//     displayCurrentAction("Drawing a filled elipse");
//     display.display();

//     delay(DELAY_MS);

//     // Code block for generating random points and sorting them in a counter
//     // clockwise direction.
//     int xt[10];
//     int yt[10];
//     int n = 10;
//     for (int i = 0; i < n; ++i)
//     {
//         xt[i] = random(100, 1100);
//         yt[i] = random(100, 725);
//     }
//     int k;
//     for (int i = 0; i < n - 1; ++i)
//         for (int j = i + 1; j < n; ++j)
//             if (atan2(yt[j] - 412, xt[j] - 600) < atan2(yt[i] - 412, xt[i] - 600))
//             {
//                 k = xt[i], xt[i] = xt[j], xt[j] = k;
//                 k = yt[i], yt[i] = yt[j], yt[j] = k;
//             }

//     // Draws a polygon, from x and y coordinate arrays of n points in color c
//     display.clearDisplay();
//     display.drawPolygon(xt, yt, n, 0);
//     displayCurrentAction("Drawing a polygon");
//     display.display();

//     delay(DELAY_MS);

//     // Fills a polygon, from x and y coordinate arrays of n points in color c,
//     // Points need to be counter clockwise sorted
//     // Method can be quite slow for now, probably will improve
//     display.clearDisplay();
//     display.fillPolygon(xt, yt, n, 0);
//     displayCurrentAction("Drawing a filled polygon");
//     display.display();

//     delay(DELAY_MS);

//     // Write text and rotate it by 90 deg. forever
//     int r = 0;
//     display.setTextColor(0, 7);
//     while (true)
//     {
//         display.setCursor(100, 100);
//         display.clearDisplay();
//         display.setRotation(
//             r); // Set rotation will sent rotation for the entire display, so you can use it sideways or upside-down
//         display.print("INKPLATE10");
//         display.display();
//         r++;
//         delay(DELAY_MS);
//     }

//     // Did you know that you can change between BW and greyscale mode anytime?
//     // Just call display.setDisplayMode(mode)
// }

void makeHorizontalLines()
{
    int lines[] = {120, 190, 400, 470, 540, 610, 680, 750};
    int length = sizeof(lines) / sizeof(lines[0]);
    int i;
    for (i = 0; i < length; i++)
    {
        drawHorizontalLine(lines[i], 0);
    }
}
void makeVerticalLines()
{
    int lines[] = {210, 320, 430, 540, 650, 760, 870, 980, 1090};
    int length = sizeof(lines) / sizeof(lines[0]);
    int i;
    for (i = 0; i < length; i++)
    {
        drawVerticalLine(lines[i], 0);
    }
}
void drawHorizontalLine(int y, int color)
{
    display.drawLine(0, y, 1200, y, color);
}
void drawVerticalLine(int x, int color)
{
    display.drawLine(x, 0, x, 190, color);
    display.drawLine(x, 400, x, 820, color);
}
void displayCurrentAction(String text)
{
    display.setCursor(50, 110);
    display.print(text);
}

void printLocalTime()
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        display.rtcSetTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);                    // Send time to RTC
        display.rtcSetDate(timeinfo.tm_wday, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year); // Send date to RTC
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S %V");

        display.rtcGetRtcData();           // Get the time and date from RTC
        seconds = display.rtcGetSecond();  // Store senconds in a variable
        minutes = display.rtcGetMinute();  // Store minutes in a variable
        hour = display.rtcGetHour();       // Store hours in a variable
        day = display.rtcGetDay();         // Store day of month in a variable
        weekday = display.rtcGetWeekday(); // Store day of week in a variable
        month = display.rtcGetMonth();     // Store month in a variable
        year = display.rtcGetYear();       // Store year in a variable

        printTime(hour, minutes, seconds, day, weekday, month, year);
    }
    else
    {
        Serial.println("Failed to obtain time");
    }
}

void printTime(uint8_t _hour, uint8_t _minutes, uint8_t _seconds, uint8_t _day, uint8_t _weekday, uint8_t _month,
               uint16_t _year)
{
    // Write time and date info on screen
    const char *wday[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    display.setCursor(20, 400);
    print2Digits(_hour);
    display.print(':');
    print2Digits(_minutes);
    display.print(':');
    print2Digits(_seconds);

    // display.print(' ');

    // display.print(wday[_weekday]);
    // display.print(", ");
    // print2Digits(_day);
    // display.print('/');
    // print2Digits(_month);
    // display.print('/');
    // display.print(_year, DEC);
}

void print2Digits(uint8_t _d)
{
    if (_d < 10)
        display.print('0');
    display.print(_d, DEC);
}

void wrapData(char *destination, String title, String appenStr)
{
    appendString(destination, (String) "<code>");
    appendString(destination, title);
    appendString(destination, appenStr);
    appendString(destination, (String) "</code>\n");
}

void appendString(char *destination, String appenStr)
{
    strcat(destination, appenStr.c_str());
}

void handleNewMessages(int numNewMessages)
{
    for (int i = 0; i < numNewMessages; i++)
    {
        Serial.print("message ID: ");
        Serial.println(bot.last_message_received);
        Serial.print(" chatID: ");
        Serial.println(bot.messages[i].chat_id);
        Serial.print(" text: ");
        Serial.println(bot.messages[i].text);
        Serial.print("text lenght = ");
        Serial.println(bot.messages[i].text.length());
        JsonDocument JFT;
        DeserializationError error = deserializeJson(JFT, bot.messages[i].text);
        if (error)
        {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }

        String type = JFT["type"].as<String>();
        String year = JFT["year"].as<String>();
        String week = JFT["week"].as<String>();
        String date = JFT["date"].as<String>();
        String matrix = JFT["matrix"].as<String>();

        char response[300]{};
        if (!type.isEmpty() && !year.isEmpty() && !week.isEmpty() && !date.isEmpty() && !matrix.isEmpty())
        {

            Serial.print("type: ");
            Serial.println(type);
            Serial.print("year: ");
            Serial.println(year);
            Serial.print("week: ");
            Serial.println(week);
            Serial.print("date: ");
            Serial.println(date);
            Serial.print("matrix: ");
            Serial.println(matrix);

            wrapData(response, "Type: ", type);
            wrapData(response, "Year: ", year);
            wrapData(response, "Week: ", week);
            wrapData(response, "Date: ", date);
            wrapData(response, "Matrix: ", matrix);

            if (bot.messages[i].hasDocument)
            {
                wrapData(response, "file_name: ", (String)bot.messages[i].file_name);
                wrapData(response, "file_size: ", (String)bot.messages[i].file_size);
                wrapData(response, "file_caption: ", (String)bot.messages[i].file_caption);

                Serial.println("file_name: " + bot.messages[i].file_name);
                Serial.println("file_size: " + bot.messages[i].file_size);
                Serial.println("file_caption: " + bot.messages[i].file_caption);
            }

            bot.sendChatAction(bot.messages[i].chat_id, "typing");
            botInfo me = bot.getMe();
            wrapData(response, "Bot_ID: ", (String)me.id);
            wrapData(response, "Bot_UName: ", (String)me.userName);
            Serial.println(response);
            Serial.println(strlen(response));
            // for (int k = 0; k < 63; k++)
            // {
            //     Serial.print(k + 1);
            //     Serial.print(": ");
            //     Serial.println(matrix[k]);
            // }
            bot.sendMessage(bot.messages[i].chat_id, response, "HTML", "true");
        }
        delay(1000);
    }
}

// typing for text messages
// upload_photo for photos
// upload_video for videos
// record_video for video recording
// upload_audio for audio files
// record_audio for audio file recording
// upload_document for general files
// find_location for location data
// upload_video_note for video notes
// record_video_note for video note recording
