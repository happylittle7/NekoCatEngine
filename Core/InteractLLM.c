#include "InteractLLM.h"
void LLMWriteScript(char* LLM_string, char all_sentence[1024][1024]) 
{
    char* line = strtok(LLM_string, "\n");
    int32_t i = 0;
    while (line != NULL && i < 1024) 
    {
        strncpy(all_sentence[i], line, 1023); 
        all_sentence[i][1023] = '\0'; 
        line = strtok(NULL, "\n"); 
        i++;
    }

    
    if (i >= 1024) 
    {
        fprintf(stderr, "Error: Exceeded the maximum number of lines (1024).\n");
    }
}

void WriteAllSentenceToFile(char all_sentence[1024][1024], const char* filename) 
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) 
    {
        perror("Error opening file");
        return;
    }

    for (int32_t i = 0; i < 1024 && all_sentence[i][0] != '\0'; i++) 
    {
        fprintf(file, "%s\n", all_sentence[i]);
    }

    fclose(file);
}

void WriteLLMString(FILE* file, char** my_LLM_string) {
    if (file == NULL || my_LLM_string == NULL) {
        fprintf(stderr, "Invalid file or string pointer.\n");
        return;
    }

    // 移动到文件末尾以确定文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 动态分配内存以存储文件内容
    *my_LLM_string = (char*)malloc(file_size + 1);  // 加1用于存储null终止符
    if (*my_LLM_string == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    // 初始化内存
    (*my_LLM_string)[0] = '\0';

    // 逐行读取文件内容并添加到my_LLM_string
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        strcat(*my_LLM_string, buffer);
        strcat(*my_LLM_string, "\n");
    }

    // 移除最后一个多余的换行符
    if (strlen(*my_LLM_string) > 0) {
        (*my_LLM_string)[strlen(*my_LLM_string) - 1] = '\0';
    }
}

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