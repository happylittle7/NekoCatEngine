#include "InteractLLM.h"


char* llm_mode(){
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
        cJSON_AddStringToObject(message1, "content", "以下是一個關於galgame引擎的教學檔案，請根據該檔案還有我題提供的assets.nekocat,item.nekocat,character.nekocat寫一段有趣的劇本");
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

}

int main(){
    llm_mode();
}