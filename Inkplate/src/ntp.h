#include <Arduino.h>
#include <time.h>
#define TIME_ZONE 0
#define DAY_LIGHT_OFF_SET 1

void connectToNTP();
void setTimeFromNTP();

time_t now;
tm myTimeInfo;

void connectToNTP()
{
    configTime(TIME_ZONE * 3600, DAY_LIGHT_OFF_SET * 3600, "0.pool.ntp.org", "1.pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    int i = 0;
    while (now < 24 * 3600)
    {
        Serial.printf("\rConnecting to NTP: %d", i);
        delay(100);
        now = time(nullptr);
        i++;
    }
    Serial.printf("\nTime gotten from NTP is OK");
};
void setTimeFromNTP()
{
    connectToNTP();
    if (getLocalTime(&myTimeInfo))
    {
        char mbstr[3];
        strftime(mbstr, sizeof(mbstr), "%V", &myTimeInfo);
        display.rtcSetWeekOfYear(atoi(mbstr));
        display.rtcSetTime(myTimeInfo.tm_hour, myTimeInfo.tm_min, myTimeInfo.tm_sec);
        display.rtcSetDate(myTimeInfo.tm_wday, myTimeInfo.tm_mday, myTimeInfo.tm_mon, 1900 + myTimeInfo.tm_year);
        display.rtcGetRtcData();

        Serial.printf("\nTIME: %d.%02d.%02d %d:(%s) %02d:%02d:%02d W(%d)\n",
                      1900 + myTimeInfo.tm_year,
                      myTimeInfo.tm_mon,
                      myTimeInfo.tm_mday,
                      myTimeInfo.tm_wday,
                      namesOfWeekDays[myTimeInfo.tm_wday],
                      myTimeInfo.tm_hour, myTimeInfo.tm_min, myTimeInfo.tm_sec,
                      atoi(mbstr));
        Serial.printf("RTC : %d.%02d.%02d %d:(%s) %02d:%02d:%02d W(%d)\n",
                      display.rtcGetYear(),
                      display.rtcGetMonth(),
                      display.rtcGetDay(),
                      display.rtcGetWeekday(),
                      namesOfWeekDays[display.rtcGetWeekday()],
                      display.rtcGetHour(),
                      display.rtcGetMinute(),
                      display.rtcGetSecond(),
                      display.rtcGetWeekOfYear());
    }
    else
    {
        Serial.println("Failed to obtain time");
    }
};
