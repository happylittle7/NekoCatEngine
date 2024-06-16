#include "InteractLLM.h"
void LLMWriteScript(char* LLM_string, char all_sentence[400][512]) 
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

    // 初始化
    size_t buffer_size = 1024;
    size_t total_size = buffer_size;
    *my_LLM_string = (char*)malloc(total_size);
    if (*my_LLM_string == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    (*my_LLM_string)[0] = '\0';  // 初始化为空字符串

    char buffer[buffer_size];
    size_t length = 0;

    // 逐行读取文件内容并添加到my_LLM_string
    while (fgets(buffer, sizeof(buffer), file)) {
        length += strlen(buffer) + 1;  // 加1用于换行符
        if (length >= total_size) {
            total_size *= 2;  // 动态增加内存
            *my_LLM_string = (char*)realloc(*my_LLM_string, total_size);
            if (*my_LLM_string == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                return;
            }
        }
        strcat(*my_LLM_string, buffer);
        strcat(*my_LLM_string, "\n");
    }

    // 移除最后一个多余的换行符
    if (strlen(*my_LLM_string) > 0 && (*my_LLM_string)[strlen(*my_LLM_string) - 1] == '\n') {
        (*my_LLM_string)[strlen(*my_LLM_string) - 1] = '\0';
    }
}

char* llm_mode(FILE* readme_file, FILE* readme2_file, FILE* assets_file, FILE* character_file, FILE* item_file){
    cJSON* cjson_test = NULL;
    cjson_test = cJSON_CreateObject();

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    char* readme;
    char* readme2;
    char* assets;
    char* character;
    char* item;
    
    WriteLLMString(readme_file, &readme);
    WriteLLMString(readme2_file, &readme2);
    WriteLLMString(character_file, &character);
    WriteLLMString(assets_file, &assets);
    WriteLLMString(item_file, &item);
    printf("%s\n",readme);
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
        cJSON_AddStringToObject(message1, "content", "以下是一個關於galgame引擎的教學檔案，請根據該檔案還有我題提供的assets.nekocat,item.nekocat,character.nekocat寫一段有趣的劇本，請直接跟我說script檔案內容即可");
        cJSON_AddItemToArray(messages, message1);

        cJSON* message2 = cJSON_CreateObject();
        cJSON_AddStringToObject(message2, "role", "user");
        cJSON_AddStringToObject(message2, "content", readme);
        cJSON_AddItemToArray(messages, message2);

        cJSON* message3 = cJSON_CreateObject();
        cJSON_AddStringToObject(message3, "role", "user");
        cJSON_AddStringToObject(message3, "content", readme2);
        cJSON_AddItemToArray(messages, message2);

        cJSON* message4 = cJSON_CreateObject();
        cJSON_AddStringToObject(message4, "role", "user");
        cJSON_AddStringToObject(message4, "content", character);
        cJSON_AddItemToArray(messages, message4);

        cJSON* message5 = cJSON_CreateObject();
        cJSON_AddStringToObject(message5, "role", "user");
        cJSON_AddStringToObject(message5, "content", assets);
        cJSON_AddItemToArray(messages, message5);

        cJSON* message6 = cJSON_CreateObject();
        cJSON_AddStringToObject(message6, "role", "user");
        cJSON_AddStringToObject(message6, "content", item);
        cJSON_AddItemToArray(messages, message6); 

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
    FILE* system_readme = fopen("Docs/system.md","r");
    if(system_readme==NULL){
        printf("ERROR!\n");
    }

    FILE* readme2 = fopen("Docs/script_actions.md","r");
    if(readme2==NULL){
        printf("ERROR!\n");
    }

    FILE* assets = fopen("assets.nekocat","r");
    if(assets==NULL){
        printf("ERROR!\n");
    }

    FILE* item = fopen("item.nekocat","r");
    if(item==NULL){
        printf("ERROR!\n");
    }

    FILE* character = fopen("character.nekocat","r");
    if(character==NULL){
        printf("ERROR!\n");
    }

    llm_mode(system_readme,readme2,assets,item,character);
    fclose(system_readme);
    fclose(readme2);
    fclose(assets);
    fclose(item);
    fclose(character);
}