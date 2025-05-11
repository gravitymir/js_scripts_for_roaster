
bool getWeather(String lat, String lon);
bool getWeather(String lat, String lon)
{
        HTTPClient http;

        //http.getStream().setNoDelay(true);
        http.getStream().setTimeout(3);
        char url[256];
        // Adare
        // "lat": 52.564,
        // "lon": -8.79
        sprintf(url, "https://api.openweathermap.org/data/2.5/forecast?lat=%s&lon=%s&lang=en&units=metric&appid=f18eb09bf809871d1b5992f7b4eafdda", lat, lon);

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
                    u8_t i = 0;
                    u8_t listLen = doc["cnt"].as<u8_t>();
                    Serial.printf("City: %s, cnt: %d\n", doc["city"]["name"].as<String>(), listLen);
                    while (i < listLen)
                    {
                        Serial.printf("%d: %.2f °C, p: %d, h: %d%%, %s, %.2f m/ps, der: %d, %s\n",
                                      i,
                                      doc["list"][i]["main"]["temp"].as<float>(),
                                      doc["list"][i]["main"]["pressure"].as<u16_t>(),
                                      doc["list"][i]["main"]["humidity"].as<u16_t>(),
                                      doc["list"][i]["weather"][0]["description"].as<const char*>(),
                                      doc["list"][i]["wind"]["speed"].as<float>(),
                                      doc["list"][i]["wind"]["deg"].as<u16_t>(),
                                      doc["list"][i]["dt_txt"].as<const char*>()
                                    );
                        i++;
                    }

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


