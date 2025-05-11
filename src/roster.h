#include "Inkplate.h"
#include <ArduinoJson.h>
#define MAKE_ROASTER_DELAY_MS 1000 * 60 * 30
// #define MAKE_ROASTER_DELAY_MS 3000

void makeRoster();
void checkTime();
bool checkTimeShowFlag = false;
void makeRosterPast(String data);
void makeRosterNow(String data);
void makeEmployeeLine(String employee);

String calculatePastData();
void drawDateCell(int row, int column, int dayOfWeek, const char date[3], bool longDayOfWeek);
void drawEmployeeNameCell(int column, String employeeName);
void printShift(int row, int column, int shift);
String *getFileNameStartMondayDate();

// String employeeNames[9] = {"Gearoid", "Peter", "Roman", "James", "Natasha", "Stan", "Calum", "Alex", "Andrew"};

const int DisplayWidht = 1200;
const int DisplayHeght = 820;
const int GridColums = 10;
const int GridRows = 15;
const int GridRestColums = DisplayWidht % GridColums;
const int GridRestRows = DisplayHeght % GridRows;
const int CellW = (DisplayWidht - GridRestColums) / GridColums + 1;
const int CellH = (DisplayHeght - GridRestRows) / GridRows + 2;
int CellsArray[GridRows * GridColums][2];
const String SHIFTS[7] = {"OFF", "9-5", "4-0", "Day", "11-9", "12-X", "2-X"};
const int shiftTextColor[7] = {6, 0, 7, 0, 7, 0, 7};
const int shiftBackColor[7] = {7, 7, 3, 3, 0, 7, 0};

unsigned long MAKE_ROASTER_LAST_TIME = MAKE_ROASTER_DELAY_MS;

String calculatePastData()
{

    return "";
}
void makeRosterPast(String data)
{
    u8_t i = 0;
    char date[3];
    char day[2];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    tm->tm_mday -= 3;

    String dataCut = data.substring(data.length() - 18, data.length());

    // Serial.println(dataCut);
    for (u8_t r = 0; r < 2; r++)
    {
        for (u8_t c = 0; c < GridColums && i < dataCut.length(); c++)
        {
            if (c % GridColums == 0)
            {
                tm->tm_mday += 1;
                mktime(tm);
                strftime(date, sizeof(date), "%d", tm);
                strftime(day, sizeof(day), "%w", tm);
                drawDateCell(r, c, *day - '0', date, false);
            }
            else
            {

                printShift(r, c, dataCut[i] - '0');
                i++;
            }
        }
    }
}
String *getFileNameStartMondayDate()
{
    String *matrix = new String[3]{"", "", ""};
    String *result = new String[3]{"", "", ""};
    if (getLocalTime(&myTimeInfo))
    {
        char fileNamePast[30];
        char fileNameNow[30];
        char fileNameNext[30];

        int offset = myTimeInfo.tm_wday;

        Serial.printf("offset: %d,  wday: %d, day: %s\n",
                      offset,
                      myTimeInfo.tm_wday,
                      namesOfWeekDays[myTimeInfo.tm_wday]);

        // go to monday
        myTimeInfo.tm_mday -= myTimeInfo.tm_wday;
        myTimeInfo.tm_mday += 1;

        mktime(&myTimeInfo);
        Serial.printf("offset: %d,  wday: %d, day: %s\n",
                      offset,
                      myTimeInfo.tm_wday,
                      namesOfWeekDays[myTimeInfo.tm_wday]);
        sprintf(fileNameNow, "%s/%d_%d_%d",
                rosterKitchenFolder,
                myTimeInfo.tm_year + 1900,
                myTimeInfo.tm_mon + 1,
                myTimeInfo.tm_mday);

        myTimeInfo.tm_mday -= 7;
        mktime(&myTimeInfo);

        sprintf(fileNamePast, "%s/%d_%d_%d",
                rosterKitchenFolder,
                myTimeInfo.tm_year + 1900,
                myTimeInfo.tm_mon + 1,
                myTimeInfo.tm_mday);

        // strftime(mbstr, sizeof(mbstr), "%V", &myTimeInfo);
        myTimeInfo.tm_mday += 14;
        mktime(&myTimeInfo);

        sprintf(fileNameNext, "%s/%d_%d_%d",
                rosterKitchenFolder,
                myTimeInfo.tm_year + 1900,
                myTimeInfo.tm_mon + 1,
                myTimeInfo.tm_mday);

        Serial.println(fileNamePast);
        Serial.println(fileNameNow);
        Serial.println(fileNameNext);

        matrix[0] = SDReadFile(fileNamePast);
        matrix[1] = SDReadFile(fileNameNow);
        matrix[2] = SDReadFile(fileNameNext);
        Serial.println(matrix[2]);
        JsonDocument JFT;
        if (matrix[0].length() > 0 && !deserializeJson(JFT, matrix[0]))
        {
            result[2] = JFT["employee"].as<String>(); // employee names
        }

        switch (offset)
        {
            // sat sun mon
        case 0:
            if (matrix[0].length() > 0 && !deserializeJson(JFT, matrix[0]))
            {
                result[0] = JFT["matrix"].as<String>().substring(36, 54);
                result[1] = JFT["matrix"].as<String>().substring(54);
            }
            if (matrix[1].length() > 0 && !deserializeJson(JFT, matrix[1]))
            {
                result[1] += JFT["matrix"].as<String>();
            }
            break;
        case 1:
            if (matrix[0].length() > 0 && !deserializeJson(JFT, matrix[0]))
            {
                result[0] = JFT["matrix"].as<String>().substring(45);
            }
            if (matrix[1].length() > 0 && !deserializeJson(JFT, matrix[1]))
            {
                result[1] = JFT["matrix"].as<String>();
            }

            break;
        case 2:
            if (matrix[0].length() > 0 && !deserializeJson(JFT, matrix[0]))
            {
                result[0] = JFT["matrix"].as<String>().substring(54);
            }
            if (matrix[1].length() > 0 && !deserializeJson(JFT, matrix[1]))
            {
                result[0] += JFT["matrix"].as<String>().substring(0, 9);
                result[1] = JFT["matrix"].as<String>().substring(9);
            }
            break;
        default:
            if (matrix[1].length() > 0 && !deserializeJson(JFT, matrix[1]))
            {
                result[0] = JFT["matrix"].as<String>().substring((offset - 3) * 9, (offset - 1) * 9);
                result[1] = JFT["matrix"].as<String>().substring((offset - 1) * 9);
            }
            break;
        }
        if (matrix[2].length() > 0 && !deserializeJson(JFT, matrix[2]))
        {
            Serial.println(JFT["matrix"].as<String>());
            Serial.println(matrix[2]);
            result[1] += JFT["matrix"].as<String>();
        }
        JFT.clear();
    }
    return result;
}
void makeEmployeeLine(String employee)
{
    display.setFont(&jet28pt7b);
    display.setTextSize(2);
    char dayNameStr[4];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    strftime(dayNameStr, sizeof(dayNameStr), "%d", tm);
    display.drawRect(0, 2 * CellH - 2, CellW, CellH * 3 - 2, 0);
    display.setCursor(-2, 3 * CellH + 40);
    display.setTextColor(0);
    display.print(String(dayNameStr).substring(0, 1));
    display.setCursor(display.getCursorX() - 8, display.getCursorY());
    display.print(String(dayNameStr).substring(1, 2));

    display.setTextColor(3);
    uint pos = 0;
    uint posLast = 0;
    int k = 0;
    while (k < 9)
    {
        pos = employee.indexOf(",", pos);
        drawEmployeeNameCell(k + 1, employee.substring(posLast, pos));
        pos += 1;
        posLast = pos;
        k++;
    }
}
void makeRosterNow(String data)
{

    u8_t i = 0;
    char date[3];
    char day[2];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    for (int r = 3; r < GridRows; r++)
    {
        for (int c = 0; c < GridColums && i < data.length(); c++)
        {
            if (r == 3)
            {
                continue;
            }
            if (c % GridColums == 0)
            {
                tm->tm_mday += 1;
                bool longDayName = false;
                if (r == 4)
                {
                    tm->tm_mday -= 1;
                    longDayName = true;
                    // Serial.printf("dayNumStr: %d => %s\n", *dayNumStr - '0', String(dateStr));
                }
                mktime(tm);
                strftime(date, sizeof(date), "%d", tm);
                strftime(day, sizeof(day), "%w", tm);
                drawDateCell(r, c, *day - '0', date, longDayName);
            }
            else
            {
                printShift(r, c, data[i] - '0');
                i++;
                // strtol(&data[i], NULL, 10)
            }
        }
    }
}
void printShift(int row, int column, int shift)
{
    display.setFont(&jet24pt7b);
    display.setTextSize(1);
    display.setCursor(column * CellW - column + 17, row * CellH - row + 45);
    display.setTextColor(0);
    if (shift == 0)
    {
        display.setTextColor(6);
    }
    if (shift == 2 || shift == 6)
    {
        display.fillRect(
            column * CellW - column,
            row * CellH - row, CellW, CellH, 0);
        display.setTextColor(7);
    }
    if (shift == 4 || shift == 5)
    {
        if (shift == 4)
        {
            display.fillRect(
                column * CellW - column,
                row * CellH - row, CellW, CellH, 3);
            display.setTextColor(7);
        }
        if (shift == 5)
        {
            display.fillRect(
                column * CellW - column,
                row * CellH - row, CellW, CellH, 5);
        }
        display.setCursor(column * CellW - column + 3, row * CellH - row + 45);
    }
    display.drawRect(
        column * CellW - column,
        row * CellH - row, CellW, CellH, 0);

    display.print(SHIFTS[shift]);
};
void drawEmployeeNameCell(int column, String employeeName)
{
    // print calendar date

    display.setCursor(column * CellW - column, 3 * CellH + 40);
    display.print(employeeName.substring(0, 1));
    display.setCursor(display.getCursorX() - 8, display.getCursorY());
    display.print(employeeName.substring(1, 2));
}
void drawDateCell(int row, int column, int dayOfWeek, const char date[3], bool longDayOfWeek)
{
    display.setFont(&JetBrainsMono_Regular24pt7b);
    display.setTextColor(2);
    display.setTextSize(1);
    String t = namesOfWeekDays[dayOfWeek].substring(0, 2) + String(date);
    display.setCursor(column + 5, row * CellH + (48 - row));

    if (longDayOfWeek)
    {
        display.setFont(&JetBrainsMono_Regular30pt7b);
        t = namesOfWeekDays[dayOfWeek].substring(0, 3);
        display.setCursor(column + 6, row * CellH + (40 - row));
        if (dayOfWeek == 1)
        {
            display.setFont(&jet30pt7b);
        }
        display.setTextColor(0);
    }
    else
    {
        display.drawRect(
            column * CellW - column,
            row * CellH - row, CellW, CellH, 0);
    }
    if (dayOfWeek == 1 && !longDayOfWeek)
    {
        display.setFont(&jet24pt7b);
        display.setTextColor(0);
    }
    display.print(t);
};
void makeRoster()
{
    display.clearDisplay();
    String *matrixArr = getFileNameStartMondayDate();
    // Serial.println("---Past---");
    // Serial.println(matrixArr[0]);
    // Serial.println("---Now ---");
    // Serial.println(matrixArr[1]);
    // Serial.println("---Employee ---");
    // Serial.println(matrixArr[2]);
    // makeRosterPast("503060102300466102003506102403606102603460102003646102430056102");
    // makeRosterNow("003056012403660112300506210603460210430066102003646102403506012003056012403606102300506102603640112003466210430066210403560012");
    makeRosterPast(matrixArr[0]);
    makeEmployeeLine(matrixArr[2]);
    makeRosterNow(matrixArr[1]);
    display.display();
};

void checkTime()
{
    display.rtcGetRtcData();
    //
    // if (display.rtcGetSecond() == 0 && checkTimeShowFlag == true)
    if (display.rtcGetMinute() == 0 || display.rtcGetMinute() == 30)
    //  if (display.rtcGetMinute() % 2)
    // if (true)
    {
        // display.rtcGetHour(),
        // display.rtcGetMinute()
        showTime();
        delay(60000);
        makeRoster();
        checkTimeShowFlag = false;
    }
    else
    {
        checkTimeShowFlag = true;
    }
}
