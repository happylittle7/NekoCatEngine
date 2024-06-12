/*
假設視覺小說類型是戀愛小說
需要支援好感度指數
增加或減少好感度的數值定義在劇本檔中的選項
需要支援玩家背包與跟物品互動的功能
*/

/*
奇妙字幕bug實驗結果 : 
和腳色圖片渲染無關，註解腳色圖片渲染程式碼，問題依然存在
和文字渲染位置無關，更改文字渲染位置，問題依然存在
和文字框渲染無關，註解文字框渲染程式，問題碼依然存在
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
static int32_t quit = 0;
// #include "final_header"

static int32_t text_start_x = 50;
static int32_t text_start_y = 370;
static int32_t dialogBox_start_x = 0;
static int32_t dialogBox_start_y = 350;
static uint8_t text_color = 0;
static int32_t character_start_x = 50;
static int32_t character_start_y = 320;
typedef struct {
    SDL_Texture* background_texture;
    SDL_Texture* dialog_box_texture;
    SDL_Rect dialog_box_renderQuad;
    SDL_Texture* text_texture;
    SDL_Rect text_renderQuad;
    SDL_Texture* expression_texture;
    SDL_Rect expression_renderQuad;
    SDL_Texture* character_IMG_texture;
    SDL_Rect* text_renderQuads; 
    int num_text_rects; 
} RenderResources;
void initRenderResources(RenderResources* resources) {
    resources->background_texture = NULL;
    resources->dialog_box_texture = NULL;
    resources->text_texture = NULL;
    resources->character_IMG_texture = NULL;
    resources->text_renderQuads = NULL;
    resources->num_text_rects = 0;
    resources->expression_texture = NULL;  // 新增
}
void freeRenderResources(RenderResources* resources) {
    if (!resources) return;

    if (resources->background_texture) {
        SDL_DestroyTexture(resources->background_texture);
        resources->background_texture = NULL;
    }
    if (resources->dialog_box_texture) {
        SDL_DestroyTexture(resources->dialog_box_texture);
        resources->dialog_box_texture = NULL;
    }
    if (resources->text_texture) {
        SDL_DestroyTexture(resources->text_texture);
        resources->text_texture = NULL;
    }
    if (resources->character_IMG_texture) {
        SDL_DestroyTexture(resources->character_IMG_texture);
        resources->character_IMG_texture = NULL;
    }
    if (resources->text_renderQuads) {
        free(resources->text_renderQuads);
        resources->text_renderQuads = NULL;
    }
    if (resources->expression_texture) 
    {
        SDL_DestroyTexture(resources->expression_texture);
        resources->expression_texture = NULL;
    }

    resources->num_text_rects = 0;
}
SDL_Texture* copyTexture(SDL_Renderer* renderer, SDL_Texture* srcTexture) //紋理深拷貝
{
    int width, height;
    SDL_QueryTexture(srcTexture, NULL, NULL, &width, &height);

    SDL_Texture* dstTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

    SDL_SetRenderTarget(renderer, dstTexture);
    SDL_RenderCopy(renderer, srcTexture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    return dstTexture;
}
// 顯示文本的函數，負責將指定文本渲染到指定位置，暫時不考慮文字太常超過對話框的h時的下拉式處理
void displayText(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w) 
{
    char *print_char = (*text);
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    int32_t now_x = *x;
    int32_t now_y = *y;
    //y+=24;
    char my_text[2] = {0};
    my_text[0] = *print_char;
    print_char++;
    *text = print_char;
    
    textSurface = TTF_RenderUTF8_Blended(font, my_text, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureAlphaMod(textTexture, 255);
    int32_t text_width = textSurface->w;  // 文本寬度
    int32_t text_height = textSurface->h; // 文本高度
    resources->text_texture = textTexture;
    SDL_Rect renderQuad = { now_x, now_y, text_width, text_height };  // 定義渲染區域
    resources->text_renderQuad = renderQuad;
    //SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
    //SDL_RenderPresent(renderer);
    SDL_Delay(50);
    now_x += text_width;
    if (now_x >= max_w)
    {
        now_y += text_height;
        now_x = *x;
    }
    *x = now_x;
    *y = now_y;
    //SDL_RenderClear(renderer);
    //SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
    SDL_FreeSurface(textSurface);  // 釋放表面資源
}

void displayText_2(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w) //滑鼠點擊後全顯示
{
    
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    char *print_char = (*text);
    textSurface = TTF_RenderUTF8_Blended(font, print_char, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureAlphaMod(textTexture, 255);
    int32_t text_width = textSurface->w;  // 文本寬度
    int32_t text_height = textSurface->h; // 文本高度
    int32_t now_x = *x;
    int32_t now_y = *y;
    if (now_x >= max_w)
    {
        now_y += text_height;
        now_x = *x;
    }
    SDL_Rect renderQuad = { now_x, now_y, text_width, text_height };  // 定義渲染區域
    resources->text_texture = textTexture;
    resources->text_renderQuad = renderQuad;
    //SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
    *print_char = 0;
    *text = print_char;
    //SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
    SDL_FreeSurface(textSurface);  // 釋放表面資源
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
void renderTexture(SDL_Renderer* renderer, SDL_Texture *tex , RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha)
{
    SDL_SetTextureAlphaMod(tex, alpha);
    resources -> expression_texture = copyTexture(renderer, tex);;
    SDL_Rect* dst = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    dst->x = x;
    dst->y = y;
    dst->w = w;
    dst->h = h;
    resources -> expression_renderQuad = *dst;
    free(dst);
}
void SetDialogBox(SDL_Renderer* renderer, SDL_Texture *tex , RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha)
{
    SDL_SetTextureAlphaMod(tex, alpha);
    resources -> dialog_box_texture = copyTexture(renderer, tex);;
    SDL_Rect* dst = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    dst->x = x;
    dst->y = y;
    dst->w = w;
    dst->h = h;
    resources -> dialog_box_renderQuad = *dst;
    free(dst);
}
void SetExpression(SDL_Renderer* renderer, SDL_Texture *tex , RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha)
{
    //SDL_SetTextureAlphaMod(tex, alpha);
    resources -> expression_texture = copyTexture(renderer, tex);;
    SDL_Rect* dst = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    dst->x = x;
    dst->y = y;
    dst->w = w;
    dst->h = h;
    resources -> expression_renderQuad = *dst;
    free(dst);
}
/*
void displayTheCharacter(SDL_Renderer* renderer, toml_table_t *Character, const char* command, const char* mood, )
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
void clearAndRender(RenderResources *resources, SDL_Texture *backgroundTexture ) 
{
    //SDL_RenderPresent(renderer);
    //SDL_Delay(2000);
    //SDL_RenderClear(renderer);
    //SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 重新渲染背景
    //renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130, 255);
    //SDL_RenderPresent(renderer);
    //SDL_RenderClear(renderer);
    //SDL_Delay(1000);  // Clear for 1 second
    resources->background_texture = backgroundTexture;
    resources->text_texture = NULL;
    //SDL_Delay(2000);

}
///顯示Expression
void DisplayTheExpression(SDL_Renderer* renderer, RenderResources *resources, toml_table_t *Character, const char* command, const char* mood) ///此段函數要在左上角印出腳色圖片
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
            
            SetExpression(renderer,character_texture, resources, character_start_x, character_start_y, 50, 50,255);
            //SDL_RenderCopy(renderer, character_texture, NULL, NULL);  // 渲染背景
            //printf("here\n");
            //SDL_DestroyTexture(character_texture);
            break;
        } 
    }
}
void my_RenderPresent(SDL_Renderer* renderer, RenderResources* resources, int32_t now_state) {
    // 清空渲染器
    
    // 渲染背景
    if (resources->background_texture) 
    {
        //printf("background_texture is OK\n");
        if (now_state == 2)
            SDL_RenderCopy(renderer, resources->background_texture, NULL, NULL);
    }
    // 渲染文字框
    if (resources->dialog_box_texture) 
    {
        //printf("dialog_box_texture is OK\n");
        if (now_state == 2)
            SDL_RenderCopy(renderer, resources->dialog_box_texture, NULL, &resources->dialog_box_renderQuad);
    }
    // 渲染角色立绘
    if (resources->character_IMG_texture) 
    {
        //printf("character_IMG_texture is OK\n");
        SDL_RenderCopy(renderer, resources->character_IMG_texture, NULL, NULL); // 此處尚未完成
    }
    // 渲染表情
    if (resources->expression_texture) 
    {
        //printf("expression_texture is OK\n");
        if (now_state == 2)
            SDL_RenderCopy(renderer, resources->expression_texture, NULL, &resources->expression_renderQuad);
    }
    // 渲染文字
    if (resources->text_texture) 
    {
        //printf("text_texture is OK\n");
        SDL_RenderCopy(renderer, resources->text_texture, NULL, &resources->text_renderQuad);
    }
    // 更新屏幕
    SDL_RenderPresent(renderer);
}
int main(int32_t argc, char* argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);  // 初始化 SDL2
    TTF_Init();  // 初始化 SDL_ttf
    // char* basePath = SDL_GetBasePath();
    // char* imagePath = (char*)calloc(1, 255);
    // snprintf(imagePath, sizeof(imagePath), "%s%s", basePath, "hello_world.png");
    // printf("imagePath: %s\n", imagePath);
    SDL_Surface* background_surface = IMG_Load("background_1.png");
   // SDL_Surface* my_ohm_Normal = IMG_Load("ohm_Normal.png");
    if (!background_surface) 
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // 设置混合模式
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_Texture* dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    //SDL_Texture* my_ohm_Normal_texture = SDL_CreateTextureFromSurface(renderer, my_ohm_Normal);
    RenderResources resources;
    initRenderResources(&resources);
    SetDialogBox(renderer, dialog_box_texture, &resources, dialogBox_start_x, dialogBox_start_y, 640, 130, 255);
    resources.background_texture = backgroundTexture;
    resources.dialog_box_texture = dialog_box_texture;

    printf("dialog_box : w=%d h=%d\n",loaded_dialog_box->w,loaded_dialog_box->h);
    printf("backgrond : w=%d h=%d\n",background_surface->w,background_surface->h);
    SDL_FreeSurface(background_surface);
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("Roboto-BoldItalic.ttf", 24);
    if (!font) 
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Color textColor;
    if(text_color)
    {
        textColor.r = 255;
        textColor.g = 255;
        textColor.b = 255;
    }
    else
    {
        textColor.r = 0;
        textColor.g = 0;
        textColor.b = 0;
    }
    ///此處以下開劇本檔
    FILE *pCharacter_file;
    FILE *pScript_file;
    pCharacter_file = fopen("charcter.toml", "r");
    if (!pCharacter_file) 
    {
        printf("Error!\n");
    }
    pScript_file = fopen("script.toml", "r");
    if (!pScript_file) 
    {
        printf("Error!\n");
    }
    char errbuf[200];
    toml_table_t *Character = toml_parse_file(pCharacter_file, errbuf, sizeof(errbuf));
    toml_table_t *Script = toml_parse_file(pScript_file, errbuf, sizeof(errbuf));
    toml_array_t *char_list = toml_array_in(Character, "char_list");
    int32_t char_Size = toml_array_nelem(char_list);
    toml_table_t *now_script = toml_table_in(Script,"Start");
    toml_array_t *script_list = toml_array_in(now_script, "script");
    ///此處以下為鍵盤互動與顯示，在此處以下，我先不會讓使用者指定背景
    int32_t now_state = 0;
    int32_t col_in_script = 0;
    char *pr_text = NULL;
    SDL_Event e;
    //SDL_RenderClear(renderer);
    //SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景
    //renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130, 255);
    int32_t print_text_x = text_start_x;
    int32_t print_text_y = text_start_y;
    int32_t had_hit_left = 0;
    my_RenderPresent(renderer,&resources,2);
    while (!quit) 
    {
        //if (run_flog)
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT) 
            {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) 
            {
                if (e.button.button == SDL_BUTTON_LEFT) 
                {
                    had_hit_left = 1;
                }
            }
        }
        if (quit == 1)
            break;
        //for (int32_t i = 0; i < toml_array_nelem(script_list); i++) 
        //printf("%d %d\n",had_hit_left,now_state);
        //printf("%d %d\n",col_in_script,toml_array_nelem(script_list));
        if (col_in_script != toml_array_nelem(script_list))
        {
            //printf("the quit = %d\n",quit);
            //SDL_RenderClear(renderer);
            
            //SDL_RenderCopy(renderer, my_ohm_Normal_texture, NULL, NULL);  // 渲染背景
            //printf("%d\n",toml_array_nelem(script_list));
            toml_table_t *entry = toml_table_at(script_list, col_in_script);
            if (!entry) continue;
            const char *action = toml_raw_in(entry, "action");
            /* 這邊要留下，不能刪
            if (IsSet(action) == 1)
            {
                if (strcmp(action, "\"SetBackground\"") == 0)
                {
                    const char *Backgroud = toml_raw_in(entry, "Backgroud");
                    background_surface = IMG_Load(Backgroud);
                    if (!background_surface) 
                    {
                        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                        return -1;
                    }
                    backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface);
                    //clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y);
                }
                else if (strcmp(action, "\"SetBackground\"") == 0)
                col_in_script++;
            }
            */
            //printf("%s\n",)
            if (strcmp(action, "\"Dialog\"") == 0)
            {
                //printf("A\n");
                const char *command = toml_raw_in(entry, "command");
                const char *text = toml_raw_in(entry, "text");
                if (text != NULL && pr_text == NULL)
                {
                    pr_text = calloc(strlen(text),sizeof(char));
                    strcpy(pr_text,text);
                    pr_text++;
                    pr_text[strlen(pr_text)-1] = 0;
                }
                if (*pr_text != 0)
                {
                    if (strcmp(command, "\"no name\"")==0)
                    {
                        if (had_hit_left == 1)
                        {
                            displayText_2(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                            now_state = 1;
                            had_hit_left = 0;
                        }
                        else
                            displayText(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                        //SDL_RenderClear(renderer);
                        //SDL_Delay(2000);
                    }
                    else
                    {
                        //printf("1\n");
                        const char *mood = toml_raw_in(entry, "mood");
                        DisplayTheExpression(renderer,&resources, Character, command, mood);
                        //SDL_RenderCopy(renderer, my_ohm_Normal_texture, NULL, NULL);  // 渲染背景
                        if (had_hit_left == 1)
                        {
                            displayText_2(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                            now_state = 1;
                            had_hit_left = 0;
                        }
                        else
                            displayText(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                    }
                }
                else if (now_state == 1 && had_hit_left == 1) //跳入下一段對話的前置作業
                {
                    now_state = 2;
                    pr_text = NULL;
                    SDL_RenderClear(renderer);
                    clearAndRender( &resources,  backgroundTexture);
                    print_text_x = text_start_x;
                    print_text_y = text_start_y;
                    had_hit_left = 0;
                }
                else
                {
                    now_state = 1;
                }
                //printf("B\n");
            }
            else if (strcmp(action, "\"SetCharacter\"")==0)
            {
                const char *label_id = toml_raw_in(entry, "label_id");
                const char *label_id = toml_raw_in(entry, "label_id");
            }
            else if (strcmp(action, "\"Option\"")==0)
            {
                col_in_script = 0;
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
                col_in_script = 0;
                //clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y); //將字跡清除
                //printf("D\n");
                
            }
            my_RenderPresent(renderer,&resources,now_state);
            if(now_state == 2) //進入下一個此場景的action
            {
                col_in_script++;
                now_state = 0;
                had_hit_left = 0;
                //clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y);
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
