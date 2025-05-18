
bool getWeather(String lat, String lon);
bool getWeather(String lat, String lon)
{
    HTTPClient http;

    // http.getStream().setNoDelay(true);
    display.clearDisplay();
    http.getStream().setTimeout(3);
    char url[256];
    // Adare
    // "lat": 52.5612,
    // "lon": -8.8155

    sprintf(url, "https://api.openweathermap.org/data/2.5/forecast?lat=%s&lon=%s&lang=en&units=metric&APPID=", lat, lon);
    // weather?lat=44.34&lon=10.99&appid=
    http.begin(url);
    JsonDocument doc;
    int httpCode = http.GET();
    if (httpCode == 200)
    {
        int32_t len = http.getSize();
        if (len > 0)
        {
            DeserializationError error = deserializeJson(doc, http.getStream());
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return 0;
            }
            else
            {
                u8_t line = 0;
                u8_t listLen = doc["cnt"].as<u8_t>();
                Serial.printf("City: %s, cnt: %d\n", doc["city"]["name"].as<String>(), listLen);
                // display.setFont(&JetBrainsMono_Regular24pt7b);
                display.setFont(&jet24pt7b);
                display.setTextSize(1);
                display.setCursor(0, 40);
                display.setTextColor(0);
                // display.print("18.19°C, p: 1018, h: 69%, scattered clouds, 05.92 m/ps, der: 101, 2025-05-13 12:00:00");
                display.print("Day Temp Pres Hum      Desc       Wind Deg");
                for (u8_t i = 0; i < listLen && line < 21; i++)
                {
                    //  Serial.printf("%5.2f°C, p: %d, h: %d%%, %16s, %5.2f m/ps, der: %3d, %s\n",
                    //               doc["list"][i]["main"]["temp"].as<float>(),
                    //               doc["list"][i]["main"]["pressure"].as<u16_t>(),
                    //               doc["list"][i]["main"]["humidity"].as<u16_t>(),
                    //               doc["list"][i]["weather"][0]["description"].as<const char *>(),
                    //               doc["list"][i]["wind"]["speed"].as<float>(),
                    //               doc["list"][i]["wind"]["deg"].as<u16_t>(),
                    //               doc["list"][i]["dt_txt"].as<const char *>());
                    const char *d = doc["list"][i]["dt_txt"].as<const char *>();
                    String h = String(d).substring(11, 13);
                    //03 06 21 00
                    if (!h.compareTo("03") ||
                        !h.compareTo("06") ||
                        !h.compareTo("21") ||
                        !h.compareTo("00"))
                    {
                        continue;
                    }
                    // Serial.printf("%5.2f°C, p: %d, h: %d%%, %16s, %5.2f m/ps, der: %3d, %s\n",
                    //               doc["list"][i]["main"]["temp"].as<float>(),
                    //               doc["list"][i]["main"]["pressure"].as<u16_t>(),
                    //               doc["list"][i]["main"]["humidity"].as<u16_t>(),
                    //               doc["list"][i]["weather"][0]["description"].as<const char *>(),
                    //               doc["list"][i]["wind"]["speed"].as<float>(),
                    //               doc["list"][i]["wind"]["deg"].as<u16_t>(),
                    //               doc["list"][i]["dt_txt"].as<const char *>());
                    display.setCursor(0, 39 * (line + 2));
                    display.printf("%4s%4.1f%5d%3d%%%17s%4.1f%4d",
                                   String(d).substring(8, 11),
                                   doc["list"][i]["main"]["temp"].as<float>(),
                                   doc["list"][i]["main"]["pressure"].as<u16_t>(),
                                   doc["list"][i]["main"]["humidity"].as<u16_t>(),
                                   doc["list"][i]["weather"][0]["description"].as<const char *>(),
                                   doc["list"][i]["wind"]["speed"].as<float>(),
                                   doc["list"][i]["wind"]["deg"].as<u16_t>());

                    // Serial.printf("%d: %05.2f °C, p: %d, h: %d%%, %16s, %05.2f m/ps, der: %d, %s\n",
                    //               i,
                    //               doc["list"][i]["main"]["temp"].as<float>(),
                    //               doc["list"][i]["main"]["pressure"].as<u16_t>(),
                    //               doc["list"][i]["main"]["humidity"].as<u16_t>(),
                    //               doc["list"][i]["weather"][0]["description"].as<const char*>(),
                    //               doc["list"][i]["wind"]["speed"].as<float>(),
                    //               doc["list"][i]["wind"]["deg"].as<u16_t>(),
                    //               doc["list"][i]["dt_txt"].as<const char*>()
                    //             );
                    line++;
                }
                display.display();
                delay(20000);

                // formatTemp(currentTemp, doc["list"][0]["main"]["temp"].as<float>() - 273.15);
                // sprintf(currentWind, "%.1f", (float)(doc["list"][0]["wind"]["speed"]));
                // strcpy(currentWeather, doc["list"][0]["weather"][0]["main"]);
                // strcpy(currentWeatherAbbr, doc["list"][0]["weather"][0]["icon"]);
                // timeZone = doc["city"]["timezone"].as<int>() / 3600;
            }
        }
    }

    return true;
};
