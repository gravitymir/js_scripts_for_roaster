#include "esp_random.h"
SdFile folder, file;
void SDWriteFile(String fileName, String data);
String SDReadFile(String fileName);
void getLSDirectory(File dir, char *dir_name, char *target);
String getLS();

// void downloadPNGFromTelegramToSD(String url, String file_name, String folder);

void printDirectory(File dir, int numTabs);
void printLS();

void playGallery();
bool displayImage(String folderPhotosPath);
int countPhotos(String folderPhotosPath);
void saveToGalery(String url);
void saveToPath(String url, String file_name);
uint16_t lastImageIndex = 0;
void saveFileToSDFromTelegram(String url, String fullPath);
void SDWriteFile(String fileName, String data)
{
    Serial.println("SDWriteFile");
    if (display.getSdCardOk())
    {
        fileName = "/" + fileName;
        if (!file.open(fileName.c_str(), FILE_WRITE))
        {
            Serial.print("SDWriteFile ");
            Serial.print(fileName);
            Serial.println(" open error");
        }
        else
        {
            Serial.printf("Writing into the file %s\n", fileName.c_str());
            file.truncate(0);
            file.truncate(100);
            file.write(data.c_str());
            file.close();
        }
    }
}
String SDReadFile(String fileName)
{
    if (display.getSdCardOk())
    {
        fileName = "/" + fileName;

        if (!file.open(fileName.c_str(), O_RDONLY))
        {
            Serial.print("SDReadFile ");
            Serial.print(fileName);
            Serial.println(" open error");
            file.close();
            return "";
        }
        else
        {
            int len = file.fileSize();
            Serial.printf("SDReadFile open: \"%s\" size: %d\n", fileName.c_str(), len);
            char text[len];
            int nbyte = file.read(text, len);
            if (nbyte == -1)
            {
                Serial.printf("SDReadFile error file.open nbyte: %d\n", nbyte);
            }
            else
            {
                text[len] = 0;
            }
            file.close();
            return text;
        }
    }
    return "";
}

void printDirectory(File dir, int numTabs)
{
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break;
        }
        for (u8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }

        entry.printName();
        // Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}
void getLSDirectory(File dir, char *dir_name, char *target)
{
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break;
        }
        char n[50]{};
        entry.getName(n, 50);
        appendString(target, dir_name);
        if (entry.isDirectory())
        {
            appendString(dir_name, n);
            appendString(dir_name, "/");
            getLSDirectory(entry, dir_name, target);
            memset(dir_name, 0, sizeof(dir_name));
            entry.close();
            continue;
        }
        appendString(target, n);
        appendString(target, "\n");
        entry.close();
    }
}
String getLS()
{
    char response[2048]{};
    File root;
    root.open("/");
    char dir[1024]{};
    getLSDirectory(root, dir, response);
    Serial.println(response);
    Serial.println("================");
    printLS();
    return response;
};
void printLS()
{
    File root;
    root.open("/");
    printDirectory(root, 0);
};
bool displayImage(String folderPhotosPath)
{
    // Get the name of the picture
    int maxCharacters = 100;
    char pictureName[maxCharacters];
    file.getName(pictureName, maxCharacters);

    // Cpoy of the folder path just for creating path for the image
    char path[maxCharacters];
    strcpy(path, folderPhotosPath.c_str());

    // Create picture path (folder path + picture name)
    char *picturePath = strcat(path, pictureName);

    // Draw the image on the screen
    if (!display.drawImage(picturePath, 0, 0, 1, 0))
    {
        // Close folder and file
        file.close();
        folder.close();

        // Return 0 to signalize an error
        return 0;
    }

    // Display the picture on the screen
    display.display();
    return 1;
}

int countPhotos(String folderPhotosPath)
{

    // If the SD card init successfully, count the files
    int fileCount = 0;

    // Open the folder
    if (folder.open(folderPhotosPath.c_str()))
    {
        // Opening the next file until it reaches the end
        while (file.openNext(&folder, O_READ))
        {
            // If the file is not hidden, increase the counter
            fileCount++;
            // Close the file
            file.close();
        }
        // Close the folder
        folder.close();
    }
    else
    {
        display.println("The folder doesn't exist");
        display.display();
    }

    // Return the number of files
    return fileCount;
}

void playGallery()
{
    int count = countPhotos(photoFolder.c_str());
    if (count)
    {
        char buffer[20];

        sprintf(buffer, "%s%d.png", photoFolder.c_str(), random(0, count));
        Serial.println(String(buffer));
        // displayImage(String(buffer));
    }
};
void saveToPath(String url, String file_name)
{
    HTTPClient http;
    http.getStream().setNoDelay(true);
    http.getStream().setTimeout(1000);
    http.begin(url);
    http.setUserAgent("curl/7.74.0");
    int httpCode = http.GET();
    if (httpCode == 200)
    {
        // Get the response length and make sure it is not 0.
        int32_t len = http.getSize();
        uint8_t buff[128] = {0};
        Serial.printf("File to save: %s", file_name.c_str());
        SdFile file;
        file.open(file_name.c_str(), FILE_WRITE);
        WiFiClient *stream = http.getStreamPtr();
        while (http.connected() && (len > 0 || len == -1))
        {
            // get available data size
            size_t size = stream->available();
            if (size)
            {
                // read up to 128 byte
                int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                file.write(buff, c);
                if (len > 0)
                {
                    len -= c;
                }
            }
            delay(1);
        }
        file.close();
    }
    http.end();
}
void saveToGalery(String url)
{
    char buffer[25];
    int count = countPhotos(photoFolder.c_str());
    sprintf(buffer, "%s%d.png", photoFolder.c_str(), count);
    saveToPath(url, String(buffer));
}


// {
//     if (file_name == "null")
//     {
//         char buffer[10];
//         int n = countPhotos(folder.c_str());
//         sprintf(buffer, "%d.%s", n + 1, url.substring(url.length() - 3, url.length()));
//         file_name = String(buffer);
//     }

//     Serial.print("---------------");
//     Serial.print(file_name);
//     Serial.println("---------------");
//     // return;
//     HTTPClient http;
//     http.getStream().setNoDelay(true);
//     http.getStream().setTimeout(1000);
//     http.begin(url);
//     http.setUserAgent("curl/7.74.0");

//     // Check response code.
//     int httpCode = http.GET();
//     if (httpCode == 200)
//     {
//         // Get the response length and make sure it is not 0.
//         int32_t len = http.getSize();
//         uint8_t buff[128] = {0};

//         SdFile file;
//         file.open(file_name.c_str(), FILE_WRITE);
//         WiFiClient *stream = http.getStreamPtr();
//         while (http.connected() && (len > 0 || len == -1))
//         {
//             // get available data size
//             size_t size = stream->available();

//             if (size)
//             {
//                 // read up to 128 byte
//                 int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

//                 file.write(buff, c);
//                 if (len > 0)
//                 {
//                     len -= c;
//                 }
//             }
//             delay(1);
//         }
//         file.close();
//     }
//     else
//     {
//         Serial.println("HTTP error");
//     }
//     http.end();
//     printLS();
// };