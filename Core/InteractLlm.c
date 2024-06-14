#include <stdio.h>
#include "../Library/cJSON.h"
#include <curl/curl.h>
#include <string.h>


char *openai_key;

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
        cJSON_AddStringToObject(data, "model", "gpt-3.5-turbo");
        cJSON* messages = cJSON_CreateArray();

        cJSON* message1 = cJSON_CreateObject();
        cJSON_AddStringToObject(message1, "role", "system");
        cJSON_AddStringToObject(message1, "content", "現在請你假裝自己為一款galgame的角色和我聊天");
        cJSON_AddItemToArray(messages, message1);

        cJSON* message2 = cJSON_CreateObject();
        cJSON_AddStringToObject(message2, "role", "assistant");
        cJSON_AddStringToObject(message2, "content", "欸欸晚餐要吃什麼呀主人");
        cJSON_AddItemToArray(messages, message2);

        cJSON* message3 = cJSON_CreateObject();
        cJSON_AddStringToObject(message3, "role", "assistant");
        cJSON_AddStringToObject(message3, "content", "還是我們去吃拉麵呢！");
        cJSON_AddItemToArray(messages, message3);

        cJSON* message4 = cJSON_CreateObject();
        cJSON_AddStringToObject(message4, "role", "assistant");
        cJSON_AddStringToObject(message4, "content", "我聽說陳冠宏很愛吃那家拉麵喔");
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