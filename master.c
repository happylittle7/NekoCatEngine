/*
假設視覺小說類型是戀愛小說
需要支援好感度指數
增加或減少好感度的數值定義在劇本檔中的選項
需要支援玩家背包與跟物品互動的功能
*/
/*
 
*/
#include "toml.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define WHITE 255, 255, 255
#define BLACK 0, 0, 0
// #include "final_header"

static int32_t text_start_x=50;
static int32_t text_start_y=370;
static int32_t dialogBox_start_x=0;
static int32_t dialogBox_start_y=350;
static uint8_t text_color = 0;
static int32_t character_start_x = 50;
static int32_t character_start_y = 320;
// 顯示文本的函數，負責將指定文本渲染到指定位置，暫時不考慮文字太常超過對話框的h時的下拉式處理
void displayText(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color textColor, int32_t x, int32_t y, int32_t max_w) 
{
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    int32_t now_x = x;
    //y+=24;
    for (int32_t i=0 ; text[i]!=0 ; i++)
    {
        char my_text[2] = {0};
        my_text[0] = text[i];
        textSurface = TTF_RenderText_Solid(font, my_text, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int32_t text_width = textSurface->w;  // 文本寬度
        int32_t text_height = textSurface->h; // 文本高度
        SDL_Rect renderQuad = { now_x, y, text_width, text_height };  // 定義渲染區域
        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
        now_x += text_width;
        if (now_x >= max_w)
        {
            y += text_height;
            now_x = x;
        }
            
    }
    //SDL_RenderClear(renderer);
    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
    
}
/*
void displayIMG(SDL_Renderer* renderer, SDL_Texture* my_texture, int32_t x, int32_t y) 
{
    //SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    //SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度
 
    SDL_Rect renderQuad = { x, y, text_width, text_height };  // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
 
    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
}
*/
///圖片縮放後再使其渲染到render
void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int32_t x, int32_t y, int32_t w, int32_t h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, tex, NULL, &dst);
}

/*
void displayTheCharacter(char* the_char, char* now_emo, char* the_text) 
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度

    SDL_Rect renderQuad = { x, y, text_width, text_height };  // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作

    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
}
*/
///清除顯示完的文字
void clearAndRender(SDL_Renderer *renderer, SDL_Texture *backgroundTexture, SDL_Texture *dialog_box_texture, 
                    int32_t dialogBox_start_x, int32_t dialogBox_start_y) 
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 重新渲染背景
    renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);  // Clear for 1 second
}
///顯示Expression
void DisplayTheExpression(SDL_Renderer* renderer, toml_table_t *Character, const char* command, const char* mood) ///此段函數要在左上角印出腳色圖片
{
    char* command_cpy = calloc(1024,sizeof(char));
    strcpy(command_cpy,command);
    command_cpy++;
    command_cpy[strlen(command_cpy)-1] = 0;
    toml_table_t *now_character = toml_table_in(Character, command_cpy);
    if (!now_character) 
    {
        fprintf(stderr, "Error: Character not found for command '%s'\n", command_cpy);
        return;
    }
    
    toml_array_t *sprites_list = toml_array_in(now_character, "sprites_list");
    if (!sprites_list) 
    {
        fprintf(stderr, "Error: Sprites list not found in character for command '%s'\n", command_cpy);
        return;
    }
    toml_array_t *sprites_png_list = toml_array_in(now_character, "sprites_png_list");
    if (!sprites_png_list) 
    {
        fprintf(stderr, "Error: Sprites list not found in character for command '%s'\n", command_cpy);
        return;
    }
    int32_t emoSize = toml_array_nelem(sprites_list);
    for (int32_t i = 0; i < emoSize; i++) 
    {
        const char* now_emo = toml_string_at(sprites_list, i).u.s;
        char* mood_copy = calloc(1024,sizeof(char));
        strcpy(mood_copy,now_emo);
        //mood_copy++;
        //mood_copy[strlen(mood_copy)-1] = 0;
        //printf("now_emo = %s\n",now_emo);
        //printf("mood_copy = %s\n",mood_copy);
        if (strcmp(mood_copy, now_emo) == 0) 
        {
            ///此處要加 character.nekocat 的路徑
            const char* now_emo_path = toml_string_at(sprites_png_list, i).u.s;
            printf("path = %s\n",now_emo_path);
            SDL_Surface* character_IMG = IMG_Load(now_emo_path);
            //printf("%s\n",now_emo);
            if (!character_IMG) 
            {
                fprintf(stderr, "Error: Unable to load image '%s': %s\n", now_emo_path, IMG_GetError());
                continue;
            }

            SDL_Texture* character_texture = SDL_CreateTextureFromSurface(renderer, character_IMG);
            SDL_FreeSurface(character_IMG);
            if (!character_texture) 
            {
                fprintf(stderr, "Error: Unable to create texture from surface: %s\n", SDL_GetError());
                continue;
            }
            renderTexture(character_texture, renderer, character_start_x, character_start_y, 50, 50);
            //SDL_RenderCopy(renderer, character_texture, NULL, NULL);  // 渲染背景
            //printf("here\n");
            SDL_DestroyTexture(character_texture);
            break;
        } 
    }
}
int main(int32_t argc, char* argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);  // 初始化 SDL2
    TTF_Init();  // 初始化 SDL_ttf
    // char* basePath = SDL_GetBasePath();
    // char* imagePath = (char*)calloc(1, 255);
    // snprintf(imagePath, sizeof(imagePath), "%s%s", basePath, "hello_world.png");
    // printf("imagePath: %s\n", imagePath);
    SDL_Surface* loadedSurface = IMG_Load("backgrond_1.png");
    SDL_Surface* my_ohm_Normal = IMG_Load("ohm_Normal.png");
    if (!loadedSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Surface* loaded_dialog_box = IMG_Load("dialog_box.png");
    if (!loaded_dialog_box) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    int32_t w_w = 640;
    int32_t w_h = 480;
    SDL_Window* window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_w, w_h, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_Texture* dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    SDL_Texture* my_ohm_Normal_texture = SDL_CreateTextureFromSurface(renderer, my_ohm_Normal);

    printf("dialog_box : w=%d h=%d\n",loaded_dialog_box->w,loaded_dialog_box->h);
    printf("backgrond : w=%d h=%d\n",loadedSurface->w,loadedSurface->h);
    SDL_FreeSurface(loadedSurface);
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
 
    TTF_Font* font = TTF_OpenFont("Roboto-Black.ttf", 24);
    if (!font) 
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
 
    SDL_Color textColor;

    if(text_color){
        textColor.r = 255;
        textColor.g = 255;
        textColor.b = 255;
    }else{
        textColor.r = 0;
        textColor.g = 0;
        textColor.b = 0;
    }
    SDL_Event e;
    int quit = 0;
    int step = 0;
    
    ///此處以下開劇本檔
    FILE *pCharacter_file;
    FILE *pScript_file;
    pCharacter_file = fopen("charcter_toml_new.txt", "r");
    if (!pCharacter_file) 
    {
        printf("Error!\n");
    }
    pScript_file = fopen("script_toml.txt", "r");
    if (!pScript_file) 
    {
        printf("Error!\n");
    }
    char errbuf[200];
    //printf("1\n");
    //printf("A\n");
    toml_table_t *Character = toml_parse_file(pCharacter_file, errbuf, sizeof(errbuf));
    //printf("2\n");
    toml_table_t *Script = toml_parse_file(pScript_file, errbuf, sizeof(errbuf));
    //printf("3\n");
    toml_array_t *char_list = toml_array_in(Character, "char_list");
    //printf("4\n");
    int32_t char_Size = toml_array_nelem(char_list);
    //printf("5\n");
    toml_table_t *now_script = toml_table_in(Script,"Start");
    //printf("6\n");
    toml_array_t *script_list = toml_array_in(now_script, "script");
    ///此處以下為鍵盤互動與顯示，在此處以下，我先不會讓使用者指定背景
    int32_t run_flog = 1;
    while (!quit) 
    {
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT) 
            {
                quit = 1;
            }
            
            if (run_flog)
            {
                for (int32_t i = 0; i < toml_array_nelem(script_list); i++) 
                {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景
                    renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);
                    //SDL_RenderCopy(renderer, my_ohm_Normal_texture, NULL, NULL);  // 渲染背景
                    
                    //printf("%d\n",toml_array_nelem(script_list));
                    
                    toml_table_t *entry = toml_table_at(script_list, i);
                    if (!entry) continue;
                    //printf("1\n");
                    const char *action = toml_raw_in(entry, "action");
                    const char *command = toml_raw_in(entry, "command");
                    const char *text = toml_raw_in(entry, "text");
                    char *pr_text;
                    if (text!=NULL)
                    {
                        pr_text = calloc(strlen(text),sizeof(char));
                        strcpy(pr_text,text);
                        pr_text++;
                        pr_text[strlen(pr_text)-1] = 0;
                    }
                    const char *mood = toml_raw_in(entry, "mood");
                    //printf("%s %d\n",action,strcmp(action, "\"Dialog\""));
                    //printf("2\n");
                    if (strcmp(action, "\"Dialog\"")==0)
                    {
                        //printf("A\n");
                        if (strcmp(command, "\"no name\"")==0)
                        {
                            displayText(renderer, font, pr_text, textColor, text_start_x, text_start_y, w_w);
                            SDL_Delay(2000);
                        }
                        else
                        {
                            //printf("1\n");
                            DisplayTheExpression(renderer, Character, command, mood);
                            //SDL_RenderCopy(renderer, my_ohm_Normal_texture, NULL, NULL);  // 渲染背景
                            displayText(renderer, font, pr_text, textColor, text_start_x, text_start_y, w_w);
                            //renderTexture(character_texture, renderer, character_start_x, character_start_y, 630, 130);
                            //SDL_RenderCopy(renderer, character_texture, NULL, NULL);  // 渲染背景
                            
                            SDL_Delay(2000);
                            //clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y); //將字跡清除
                        }
                        //printf("B\n");
                    }
                    else if (strcmp(action, "\"Jump\"")==0)
                    {
                        //printf("C\n");
                        const char *label_id = toml_raw_in(entry, "label_id");
                        char *label_id_copy = calloc(1024,sizeof(char));
                        strcpy(label_id_copy,label_id);
                        label_id_copy++;
                        label_id_copy[strlen(label_id_copy)-1] = 0;
                        printf("label_id = %s\n",label_id_copy);
                        now_script = toml_table_in(Script,label_id_copy);
                        script_list = toml_array_in(now_script, "script");
                        //clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y); //將字跡清除
                        //printf("D\n");
                        break;
                    }
                    if (toml_array_nelem(script_list)==1)
                    {
                        run_flog = 0;
                        break;
                    }
                    //SDL_RenderPresent(renderer);
                    //SDL_RenderClear(renderer);
                    clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y);
                }
                
            }
            
        }
        
    }
    
 
    TTF_CloseFont(font);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
