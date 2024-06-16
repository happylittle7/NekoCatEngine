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