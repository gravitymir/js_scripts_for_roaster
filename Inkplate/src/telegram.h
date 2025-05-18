#include <UniversalTelegramBot.h>
// #define BOT_TOKEN "" // 
#define BOT_TOKEN "" // 
#define TELEGRAM_BOT_DELAY_MS 10000
unsigned long TELEGRAM_BOT_LAST_TIME;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void telegramUpdate();
void handleNewMessages(int numNewMessages);

void telegramUpdate()
{
    if (millis() - TELEGRAM_BOT_LAST_TIME > TELEGRAM_BOT_DELAY_MS)
    {
        Serial.println("telegramUpdate();");
        if (WiFi.status() == WL_CONNECTED)
        {
            int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            while (numNewMessages)
            {
                handleNewMessages(numNewMessages);
                numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }
        }
        TELEGRAM_BOT_LAST_TIME = millis();
    }
}

void handleNewMessages(int numNewMessages)
{
    for (int i = 0; i < numNewMessages; i++)
    {
        telegramMessage &msg = bot.messages[i];
        if (msg.from_id != "283404954")
        {
            return;
        }
        bot.sendChatAction(msg.chat_id, "typing");

        if (msg.text == "/ls")
        {
            String resp = getLS();
            bot.sendMessage(msg.chat_id, resp, "HTML");
            return;
        }
        char response[1024]{};
        // botInfo me = bot.getMe();
        // wrapData(response, "Bot_ID: ", me.id);
        // wrapData(response, "Bot_Nm: ", me.userName);
        wrapData(response, "ChatID: ", msg.chat_id);
        wrapData(response, "FromID: ", msg.from_id);
        wrapData(response, "FromNm: ", msg.from_name);
        Serial.print("message ID: ");
        Serial.println(bot.last_message_received);
        Serial.print(" chatID: ");
        Serial.println(msg.chat_id);
        Serial.print(" text: ");
        Serial.println(msg.text);
        Serial.print("text lenght = ");
        Serial.println(msg.text.length());
        JsonDocument JFT;
        if (deserializeJson(JFT, msg.text))
        {
            Serial.println("deserializeJson() failed");
        }
        else if (!msg.hasDocument)
        {
            String type = JFT["type"].as<String>();
            String period = JFT["period"].as<String>();
            String employee = JFT["employee"].as<String>();
            String matrix = JFT["matrix"].as<String>();
            String output;
            JFT.remove("type");
            JFT.remove("period");
            serializeJson(JFT, output);
            JFT.clear();
            if (!period.isEmpty() && !matrix.isEmpty() && !type.isEmpty())
            {
                char path[30];
                if (type == "kitchen")
                {
                    sprintf(path, "%s%s", rosterKitchenFolder, period);
                    SDWriteFile(path, output);
                    makeRoster();
                }

                // String respStr = SDReadFile(period);
                // Serial.println("<---SDReadFile");
                // Serial.println(respStr);
                // Serial.println("------------->");
                wrapData(response, "Period: ", period);
                wrapData(response, "Employee: ", employee);
                wrapData(response, "Matrix: ", matrix);

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
                //
            }
        }
        if (msg.hasDocument)
        {
            Serial.println("hasDocument");
            strcat(response, "<a href=\"");
            strcat(response, msg.file_path.c_str());
            strcat(response, "\">Link</a>\n");
            wrapData(response, "file_name: ", msg.file_name);
            wrapData(response, "file_size: ", (String)msg.file_size);
            wrapData(response, "file_caption: ", msg.file_caption);
            // display.drawBitmapFromWeb(msg.file_path);
            // display.drawImage(msg.file_path, 0, 0, true, false);
            // display.display();
            // delay(10000);
            // myDrawJpegFromWeb(msg.file_path.c_str(), 0, 0, true, false);
            JsonDocument JFT;

            Serial.println(msg.file_caption);
            String command;
            String file_name;
            if (!deserializeJson(JFT, msg.file_caption))
            {
                bot.sendChatAction(msg.chat_id, "typing");
                String command = JFT["command"].as<String>();

                if (command == "savephoto" || command == "savephotoshow")
                {
                    saveToGalery(msg.file_path, command == "savephotoshow");
                }
                String path = JFT["path"].as<String>();
                bool show = JFT["show"].as<bool>();
                Serial.print(command);
                Serial.print(path);
                Serial.print(show);
                if (command == "savefile" && !path.isEmpty() && show)
                {
                    saveToPath(msg.file_path, path, show);
                }
                JFT.clear();
            }

            // Serial.println("file_name: " + msg.file_name);
            // Serial.println("file_size: " + msg.file_size);
            // Serial.println("file_caption: " + msg.file_caption);
        }
        else
        {
            wrapData(response, "text: ", msg.text);
        }
        bot.sendMessage(msg.chat_id, response, "HTML");
    }
}
