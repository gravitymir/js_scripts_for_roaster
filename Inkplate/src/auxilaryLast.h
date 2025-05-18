void checkTime();
void checkTime()
{
    display.rtcGetRtcData();

    u8_t h = display.rtcGetHour();
    u8_t m = display.rtcGetMinute();
    u8_t s = display.rtcGetSecond();

    if (h == 23)
    {
        int count = countPhotos(logoFolderPath.c_str());
        char buffer[20];
        sprintf(buffer, "%s/%d.jpeg", logoFolderPath, random(0, count));
        Serial.println(buffer);
        display.clearDisplay();
        display.drawImage(buffer, 0, 0, 1, 0);
        display.display();
        timeToSleep(10, 0, 0);
        return;
    }
    
    if (m == 0)
    {
        showTime();
        delay(60000);
        getWeather("52.5612", "-8.8155");
        delay(60000);
        telegramUpdate();
        makeRoster();
        timeToSleep(0, 27, 0);
        return;
    }
    if (m == 30)
    {
        showTime();
        delay(30000);
        int count = countPhotos(galeryFolderPath.c_str());
        if (h == 14  && count > 50)
        {
            playGallery();
            delay(20000);
        }
        getWeather("52.5612", "-8.8155");
        delay(30000);
        telegramUpdate();
        makeRoster();
        timeToSleep(0, 28, 30);
        return;
    }
}