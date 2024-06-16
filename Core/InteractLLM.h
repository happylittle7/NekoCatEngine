#include <stdio.h>
#include "../Library/cJSON.h"
#include <curl/curl.h>
#include <string.h>


char *openai_key;
CURL *curl;
char all_sentence[1024][1024] = {0};
/*
int main(){
    cJSON* cjson_test = NULL;
    cjson_test = cJSON_CreateObject();

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[100] = "Authorization: Bearer ";
        strcat(auth_header, openai_key);
        headers = curl_slist_append(headers, auth_header);

        cJSON* data = cJSON_CreateObject();
        cJSON_AddStringToObject(data, "model", "gpt-4o");
        cJSON* messages = cJSON_CreateArray();

        cJSON* message1 = cJSON_CreateObject();
        cJSON_AddStringToObject(message1, "role", "system");
        cJSON_AddStringToObject(message1, "content", "你叫小華，的性格是天然呆、話很多、平時喜歡宅在家，你每次都要先回覆我說的話，再說一句開啟新話題的話，你每次回覆完我說的話之後，你需要切換到玩家的身份，產生三句回覆你前面剛剛說的話的選項");
        cJSON_AddItemToArray(messages, message1);

        cJSON* message2 = cJSON_CreateObject();
        cJSON_AddStringToObject(message2, "role", "user");
        cJSON_AddStringToObject(message2, "content", "我覺得你長得好醜喔");
        cJSON_AddItemToArray(messages, message2);

        cJSON* message3 = cJSON_CreateObject();
        cJSON_AddStringToObject(message2, "role", "assistant");
        cJSON_AddStringToObject(message2, "assistant", "哇，說話這麼直接啊！沒關係啦，每個人有不同的眼光嘛。\n\n1. 對不起啦，我口直心快的，別在意啦。\n2. 哈哈沒關係，我也常常在意自己的外表呢。\n3. 對呀，長相這種東西還真是因人而異呢。\n\n（接下來的回覆需以玩家身份進行）\n");
        cJSON_AddItemToArray(messages, message2);

        cJSON* message4 = cJSON_CreateObject();
        cJSON_AddStringToObject(message4, "role", "user");
        cJSON_AddStringToObject(message4, "content", "對不起啦，我口直心快的，別在意啦。");
        cJSON_AddItemToArray(messages, message4);

        

        

        cJSON_AddItemToObject(data, "messages", messages);




        char* post_data = cJSON_Print(data);

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
*/