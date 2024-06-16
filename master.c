/*
假設視覺小說類型是戀愛小說
需要支援好感度指數
增加或減少好感度的數值定義在劇本檔中的選項
需要支援玩家背包與跟物品互動的功能
*/

/*
奇妙字幕bug實驗結果 : -> 不知為何已經解決了
和腳色圖片渲染無關，註解腳色圖片渲染程式碼，問題依然存在
和文字渲染位置無關，更改文字渲染位置，問題依然存在
和文字框渲染無關，註解文字框渲染程式，問題碼依然存在
*/
#include "./Core/dialog.h"
#include "./Core/backpack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
/*
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
typedef struct 
{
    SDL_Rect rect;
    SDL_Color color;
    const char* text;
    TTF_Font* font;
    SDL_Texture* texture;
} Button;
typedef struct 
{
    SDL_Texture* background_texture;
    SDL_Texture* dialog_box_texture;
    SDL_Rect dialog_box_renderQuad;
    SDL_Texture* text_texture;
    SDL_Rect text_renderQuad;
    SDL_Texture* expression_texture;
    SDL_Rect expression_renderQuad;
    SDL_Texture** character_IMG_texture;
    SDL_Rect** character_IMG_renderQuads;
    Button** now_option_button;
    Button* leave_button;
} RenderResources;
*/

int main(int32_t argc, char* argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);  // 初始化 SDL2
    TTF_Init();  // 初始化 SDL_ttf
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* loaded_dialog_box = IMG_Load("./assets.nekocat/dialog_box.png"); ///這裡的路徑之後會有變化
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
    SDL_Renderer *backpack_renderer = NULL;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // 设置混合模式
    RenderResources resources;
    RenderResources backpack_resources;
    initRenderResources(&resources);
    initRenderResources(&backpack_resources);
    //resources.background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    resources.dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    //SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, background_surface);
    //SDL_Texture* dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    //SDL_Texture* my_ohm_Normal_texture = SDL_CreateTextureFromSurface(renderer, my_ohm_Normal);
    SetDialogBox(renderer, &resources, dialogBox_start_x, dialogBox_start_y, 640, 130, 255);
    //resources.background_texture = backgroundTexture;
    //resources.dialog_box_texture = dialog_box_texture;

    //printf("dialog_box : w=%d h=%d\n",loaded_dialog_box->w,loaded_dialog_box->h);
    //printf("backgrond : w=%d h=%d\n",background_surface->w,background_surface->h);
    
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    char *font_path = "./Assets/Font/Cubic_11_1.100_R.ttf"; ///這裡的路徑之後會有變化
    TTF_Font* font = TTF_OpenFont(font_path, 24); ///這裡的路徑之後會有變化
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
    FILE *pPlayer_sav_file;
    FILE *pbackground_file;
    FILE *pmusic_file;
    pCharacter_file = fopen("./character.nekocat/charcter.toml", "r");
    if (!pCharacter_file) 
    {
        printf("Error!\n");
    }
    pScript_file = fopen("script.toml", "r");
    if (!pScript_file) 
    {
        printf("Error!\n");
    }
    pPlayer_sav_file = fopen("player.sav", "r");
    if (!pPlayer_sav_file) 
    {
        printf("Error!\n");
    }
    pbackground_file = fopen("./assets.nekocat/background.toml", "r");
    if (!pbackground_file) 
    {
        printf("Error!\n");
    }
    pmusic_file = fopen("./assets.nekocat/music.toml", "r");
    if (!pmusic_file) 
    {
        printf("Error!\n");
    }
    
    char errbuf[200];
    toml_table_t *Character = toml_parse_file(pCharacter_file, errbuf, sizeof(errbuf));
    toml_table_t *Script = toml_parse_file(pScript_file, errbuf, sizeof(errbuf));
    toml_table_t *Player_V = toml_parse_file(pPlayer_sav_file, errbuf, sizeof(errbuf));
    toml_table_t *background_table = toml_parse_file(pbackground_file, errbuf, sizeof(errbuf));
    toml_table_t *music_table = toml_parse_file(pmusic_file, errbuf, sizeof(errbuf));
    background_table = toml_table_in(background_table,"background");
    music_table = toml_table_in(music_table,"sound");
    toml_array_t *char_list = toml_array_in(Character, "char_list");
    int32_t char_Size = toml_array_nelem(char_list);
    toml_table_t *now_script = toml_table_in(Script,"Start");
    toml_table_t *Player_Variable = toml_table_in(Player_V,"Variable");
    
    if (!Player_Variable) 
    {
        printf("Error parsing player.sav: %s\n", errbuf);
        return 1;
    }
    toml_table_t *Player_Items = toml_table_in(Script,"Items");
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
    int32_t now_handle = 3;
    ////音樂處理
    if (!init_music()) 
    {
        printf("Failed to initialize!\n");
        return 1;
    }
    Mix_Music* bgm = NULL;
    //my_RenderPresent(renderer,&resources,2);
    // background



    // printf("test 09\n");
    while (!quit) 
    {
        //if (run_flog)
        while (SDL_PollEvent(&e) != 0) 
        {
            switch(e.type){
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    // printf("test 02\n");
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_e:
                            // printf("test 03\n");
                            if(now_state != 4){
                                now_state = 4;
                            }
                            else if(now_state == 4){
                                SDL_RenderClear(renderer);
                                clearAndRender( &resources,  resources.background_texture );
                                
                                now_state = 6;
                                had_hit_left = 0;
                            }
                            // printf("now_state = %d\n",now_state);
                            break;
                        default:
                            break;
                    }
                    // printf("now_state = %d\n",now_state);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(e.button.button == SDL_BUTTON_LEFT){
                        had_hit_left = 1;
                    }
                    break;
                default:
                    break;
            }
        }
        if (quit == 1)
            break;
        //for (int32_t i = 0; i < toml_array_nelem(script_list); i++) 
        //printf("%d %d\n",had_hit_left,now_state);
        //printf("%d %d\n",col_in_script,toml_array_nelem(script_list));
        printf("now_state = %d\n",now_state);
        if(now_state == 4)
        {
            // SDL_DestroyRenderer(renderer);
            
            SDL_RenderClear(renderer);
            // backpack_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            if (!renderer)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return -1;
            }
            // printf("test 01\n");
            
            
            // Initialize backpack
            backpackData backpack_data;
            backpackItem *backpack_items;
            backpackPath backpack_path;
            char *background_path = "./Assets/background/background_01.png";
            // Initialize backpack_data
            
            backpackDataInit(&backpack_data);
            // printf("test 04\n");
            // Initialize backpack_path
            backpack_path.background_path = calloc(1024, sizeof(char));
            backpack_path.font_path = calloc(1024, sizeof(char));
            backpack_path.black_block_path = calloc(1024, sizeof(char));
            backpack_path.white_edge_black_block_path = calloc(1024, sizeof(char));
            strcpy(backpack_path.background_path, background_path);
            strcpy(backpack_path.font_path, font_path);
            strcpy(backpack_path.black_block_path, "./Assets/image/black_block.png");
            strcpy(backpack_path.white_edge_black_block_path, "./Assets/image/white_edge_black_block.png");
            // printf("test 05\n");
            // Initialize backpack_items
            backpack_items = calloc(6,sizeof(backpackItem));
            for(int32_t i = 0;i < 6;i++){
                backpack_items[i].name = calloc(1024,sizeof(char));
                backpack_items[i].description = calloc(1024,sizeof(char));
                backpack_items[i].image_path = calloc(512,sizeof(char));
            }
            // Initialize backpack_items
            int32_t *status;
            status = calloc(6,sizeof(int32_t));
            status[0] = 0;
            status[1] = 1;
            status[2] = 1;
            status[3] = 1;
            status[4] = 0;
            status[5] = 0;
            if(now_handle >= 0 && now_handle < 6){
                if(status[now_handle] == 1)status[now_handle] = 2;
            }
            char **name = calloc(6,sizeof(char*));
            name[0] = NULL;
            name[1] = "我的橡皮擦";
            name[2] = "警長巴巴的鉛筆";
            name[3] = "艾西莫夫的水壺";
            name[4] = NULL;
            name[5] = NULL;
            char **description = calloc(6,sizeof(char*));
            description[0] = NULL;
            description[1] = "這是我的橡皮擦,我很喜歡";
            description[2] = "這是我的橡皮擦,我很喜歡";
            description[3] = "這是艾西莫夫的未來水壺,遵守機器人三大法則,相信這是common sense吧!";
            description[4] = NULL;
            description[5] = NULL;
            // char *description[6] = {NULL, "我的橡皮擦", "警長巴巴的鉛筆", "艾西莫夫的水壺", NULL, NULL};
            char **image_path = calloc(6,sizeof(char*));
            image_path[0] = NULL;
            image_path[1] = "eraser.png";
            image_path[2] = "pencil.png";
            image_path[3] = "waterbuttle.png";
            image_path[4] = NULL;
            image_path[5] = NULL;
            // char *image_path[6] = {NULL, "eraser.png", "pencil.png", "waterbuttle.png", NULL, NULL};
            // printf("test 06\n");
            
            //IMG_Load("./item.nekocat/eraser.png");
            //IMG_Load("./item.nekocat/eraser.png");
            int32_t check = -10;
            // for(int32_t i = 0;i < 6;i++){
            //     backpack_items[i].status = status[i];
            //     printf("backpack_items[%d].status = %d\n",i,backpack_items[i].status);
            // //     printf("test 06-1\n");
            //     strcpy(backpack_items[i].name, name[i]);
            // //     printf("test 06-2\n");
            //     strcpy(backpack_items[i].description, description[i]);
            // //     printf("test 06-3\n");
            //     strcpy(backpack_items[i].image_path, image_path[i]);
            // //     printf("test 06-4\n");
            // }
            // printf("test 06-5\n");
            // check = backpackItemInit(&backpack_items, status, name, description, image_path); //此處之後不能IMG_Load
            if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                return -1;
            }
            else
            {
                printf("SDL_image initialize success!\n");
            }
            // printf("test 08-1\n");
            SDL_Surface *background_surface = IMG_Load("./Assets/background/background_01.png");
            // printf("test 08-2\n");
            if (!background_surface)
            {
                printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                return -1;
            }
            backpack_resources.background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
            SDL_FreeSurface(background_surface);

            // backpack_block
            // printf("test 08-5\n");
            SDL_Surface *backpack_block_surface = IMG_Load("./Assets/image/black_block.png");
            if (!backpack_block_surface)
            {
                printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                SDL_DestroyRenderer(backpack_renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return -1;
            }
            backpack_resources.backpack_block_texture = SDL_CreateTextureFromSurface(renderer, backpack_block_surface);
            SDL_SetTextureAlphaMod(backpack_resources.backpack_block_texture, 200); // 調整透明度
            SDL_FreeSurface(backpack_block_surface);
            // printf("test 08-6\n");

            // white_edge_black_block
            SDL_Surface *white_edge_black_block_surface = IMG_Load("./Assets/image/white_edge_black_block.png");
            if (!white_edge_black_block_surface)
            {
                printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                SDL_DestroyRenderer(backpack_renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return -1;
            }
            backpack_resources.white_edge_black_block_texture = SDL_CreateTextureFromSurface(renderer, white_edge_black_block_surface);
            SDL_SetTextureAlphaMod(backpack_resources.white_edge_black_block_texture, 200); // 調整透明度
            SDL_FreeSurface(white_edge_black_block_surface);
            // printf("test 08-7\n");

            // 测试加载图像
            SDL_Surface *temp_surface = IMG_Load("./item.nekocat/eraser.png");
            if (!temp_surface) {
                printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                IMG_Quit();
                return -1;
            }
            SDL_FreeSurface(temp_surface);
            
            // printf("test 07\n");
            
            
            // Backpack main
            // RenderResources backpack_resources;
            // initRenderResources(&backpack_resources);
            
            // printf("test 08\n");
            // printf("test 08-6\n");
            if (backpack_resources.item_texture==NULL)
            {
                resources.item_texture = (SDL_Texture**)malloc(6 * sizeof(SDL_Texture*));
                for (int i = 0; i < 6; ++i) 
                {
                    resources.item_texture[i] = NULL;  // 分配内存 
                    resources.item_renderQuads[i] = (SDL_Rect*)malloc(sizeof(SDL_Rect));  // 分配内存
                }
            }
            for(int32_t i = 0; i < 6; i++){
                // printf("items[%d].status = %d\n", i, status[i]);
                if(status[i] == 0){
                    continue;
                }
                char *temp;
                temp = (char *)malloc(sizeof(char) * 100);
                snprintf(temp, 100, "./item.nekocat/%s", image_path[i]);
                // printf("temp = %s\n", temp);
                // printf("test 08-6-1\n");
                SDL_Surface *item_surface = IMG_Load(temp);
                //SDL_Surface *item_surface = NULL;
                // printf("test 08-6-2\n");
                if (!item_surface)
                {
                    printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                    SDL_DestroyRenderer(backpack_renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return -1;
                }
                // printf("test 08-6-3\n");
                
                SDL_Texture *temp_tex = SDL_CreateTextureFromSurface(renderer, item_surface);
                // printf("test 08-6-4\n");
                if (backpack_resources.item_texture==NULL)
                {
                    // printf("id = %d, backpack_resources.item_texture is NULL\n",i);
                }
                else
                {
                    // printf("id = %d, backpack_resources.item_texture is not NULL\n",i);
                }
                backpack_resources.item_texture[i] = temp_tex;
                // printf("test 08-6-5\n");
                SDL_FreeSurface(item_surface);
            }
            
            int32_t result = backpackMain(&backpack_data, status, name, description, image_path, &backpack_path, &backpack_resources, renderer, font, window);
            // switch(result){
            //     case -1:
            //         printf("Failed to initialize backpack!\n");
            //         break;
            //     case 0:
            //         printf("Leave the game\n");
            //         now_state = 0;
            //         quit = 1;
            //         break;
            //     case 7:
            //         printf("Backpack closed!\n");
            //         now_state = 0;
            //         break;
            //     case 1 ... 6:
            //         printf("Item %d used!\n", result);
            //         now_handle = result - 1;
            //         now_state = 0;
            //         break;
            // }
            my_RenderPresentForBackpack(renderer, &backpack_resources, now_state, &backpack_data, status, name, description, image_path, font, window);
            //freeRenderResources(&backpack_resources);
            continue;
        }
        
        if (col_in_script != toml_array_nelem(script_list) && now_state != 4)
        {
            //printf("the quit = %d\n",quit);
            //SDL_RenderClear(renderer);
            
            //SDL_RenderCopy(renderer, my_ohm_Normal_texture, NULL, NULL);  // 渲染背景
            //printf("%d\n",toml_array_nelem(script_list));
            toml_table_t *entry = toml_table_at(script_list, col_in_script);
            if (!entry) continue;
            const char *action = toml_raw_in(entry, "action");
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
                        else if (now_state == 6)
                        {
                            *pr_text = 0;
                            now_state = 1;
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
                        DisplayTheExpression(renderer, &resources, Character, command, mood);
                        //SDL_RenderCopy(renderer, my_ohm_Normal_texture, NULL, NULL);  // 渲染背景
                        if (had_hit_left == 1)
                        {
                            displayText_2(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                            now_state = 1;
                            had_hit_left = 0;
                        }
                        else if (now_state == 6)
                        {
                            *pr_text = 0;
                            now_state = 1;                     
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
                    //freeRenderResources(&resources);
                    //initRenderResources(&resources);
                    //SetDialogBox(renderer, dialog_box_texture, &resources, dialogBox_start_x, dialogBox_start_y, 640, 130, 255);
                    //resources.background_texture = backgroundTexture;
                    //resources.dialog_box_texture = dialog_box_texture;
                    clearAndRender( &resources,  resources.background_texture );
                    print_text_x = text_start_x;
                    print_text_y = text_start_y;
                    had_hit_left = 0;
                }
                else if (now_state == 6)
                {
                    displayText_3(renderer,&resources, font, &text, textColor, &print_text_x, &print_text_y, w_w);
                    my_RenderPresent(renderer,&resources,now_state);
                    now_state = 1;
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
                toml_datum_t character_number = toml_int_in(entry, "number");
                toml_array_t* command_array = toml_array_in(entry, "command_list");
                toml_array_t* mood_array = toml_array_in(entry, "mood_list");
                if (resources.character_IMG_texture == NULL)
                {
                    printf("character_IMG_texture is NULL\n");
                    //resources.character_IMG_renderQuads = calloc(3,sizeof(SDL_Rect*));
                }
                else
                {
                    printf("character_IMG_texture is not NULL\n");
                }
                if (character_number.u.i == 0)
                {
                    for (int32_t i=0 ; i<3 ; i++) ///初始化character_IMG_texture不然會出事
                    {
                        resources.character_IMG_texture[i] = NULL;
                        //resources.character_IMG_renderQuads[i] = NULL;
                    }
                }
                else
                {
                    for (int32_t i=0 ; i<3 ; i++) ///初始化character_IMG_texture不然會出事
                    {
                        resources.character_IMG_texture[i] = NULL;
                        //resources.character_IMG_renderQuads[i] = NULL;
                    }
                    for (int32_t i = 0 ; i < toml_array_nelem(command_array) ; i++)
                    {
                        const char* Character_id = toml_string_at(command_array, i).u.s;
                        const char* mood = toml_string_at(mood_array, i).u.s;
                        //printf("%s\n",Character_id);
                        //printf("%s\n",mood);
                        displayIMG(renderer, &resources, Character, Character_id, mood, i, toml_array_nelem(command_array)); 
                    }
                }
                col_in_script++;
            }
            else if (strcmp(action, "\"Option\"")==0) 
            {
                //{ action = "Option", optionCount = 3, optionContent = ["Button1","Button2","Button3"], optionJump = ["label_id_1","label_id_2","label_id_3"]}

                col_in_script = 0;
            }
            else if (strcmp(action, "\"Jump\"") == 0)
            {
                //printf("C\n");
                const char *label_id = toml_raw_in(entry, "label_id");
                char *label_id_copy = calloc(1024,sizeof(char));
                strcpy(label_id_copy,label_id);
                label_id_copy++;
                label_id_copy[strlen(label_id_copy)-1] = 0;
                toml_array_t* variable_array = toml_array_in(entry, "variable");
                toml_array_t* mode_array = toml_array_in(entry, "mode");
                toml_array_t* gap_array = toml_array_in(entry, "gap");
                if (!variable_array)
                {
                    if (IsOverGap(Player_Variable, variable_array,mode_array,gap_array) == 1)
                    {
                        //printf("1\n");
                        //printf("%s\n",label_id_copy);
                        now_script = toml_table_in(Script,label_id_copy);
                        //printf("1\n");
                        script_list = toml_array_in(now_script, "script");
                        //printf("1\n");
                        col_in_script = 0;
                        //printf("1\n");
                        //had_hit_left = 0;
                    }
                    else
                    {
                        col_in_script++;
                        //had_hit_left = 0;
                    }
                }
                else
                {
                    now_script = toml_table_in(Script,label_id_copy);
                    script_list = toml_array_in(now_script, "script");
                    col_in_script = 0;
                }
                //clearAndRender( renderer,  backgroundTexture,  dialog_box_texture, dialogBox_start_x, dialogBox_start_y); //將字跡清除
                //printf("D\n");
            }
            else if (strcmp(action, "\"Background\"") == 0)
            {
                const char *background_v = toml_raw_in(entry, "background");
                char* background_v_copy = calloc(1024,sizeof(char));
                strcpy(background_v_copy,background_v);
                background_v_copy++;
                background_v_copy[strlen(background_v_copy)-1] = 0;
                
                ///////////此處尚未完成
                //printf("%s\n",background_v_copy);
                toml_datum_t background_path_datum = toml_string_in(background_table, background_v_copy);
                if (!background_path_datum.ok) 
                {
                    printf("Error: 'background_1' not found in 'background' table\n");
                }
                ///這裡的路徑之後會有變化
                char full_path[512] = {0};
                snprintf(full_path, sizeof(full_path), "./Assets/background/%s", background_path_datum.u.s);

                SDL_Surface* background_surface = IMG_Load(full_path);
                if (!background_surface) 
                {
                    printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                    return -1;
                }
                resources.background_texture = SDL_CreateTextureFromSurface(renderer,background_surface);
                SDL_FreeSurface(background_surface);
                now_state = 2;
            }
            else if (strcmp(action, "\"Music\"") == 0)
            {
                const char* music_id = toml_raw_in(entry, "music_id");
                char* music_id_copy = calloc(1024,sizeof(char));
                strcpy(music_id_copy,music_id);
                music_id_copy++;
                music_id_copy[strlen(music_id_copy)-1] = 0;
                toml_datum_t music_path_datum = toml_string_in(music_table, music_id_copy);
                if (bgm != NULL)
                {
                    Mix_HaltMusic();
                    Mix_FreeMusic(bgm);
                }
                if (strcmp(music_path_datum.u.s,"close") != 0)
                {
                    char full_path[512] = {0};
                    snprintf(full_path, sizeof(full_path), "./Assets/sound/%s", music_path_datum.u.s);
                    bgm = Mix_LoadMUS(full_path);
                    if (bgm == NULL) 
                    {
                        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
                        break;
                    }
                    if (Mix_PlayMusic(bgm, -1) == -1) 
                    {
                        printf("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
                        Mix_FreeMusic(bgm);
                        break;
                    }
                }
                now_state = 2;
            }
            // printf("now_state = %d\n",now_state);
            
            //printf("here\n");
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
    //SDL_DestroyTexture(backgroundTexture);
    //printf("A\n");
    freeRenderResources(&resources);
    //printf("B\n");
    SDL_DestroyRenderer(renderer);  //這行因為未知原因若不註解掉會有機率錯，但走到這行程式必定結束，資源必定會釋放，所以我註解掉了
    /*
    if (renderer)
    {
        printf("renderer NO problem!\n");
        SDL_DestroyRenderer(renderer);
    }
    else
    {
        printf("renderer has problem!\n");
    }
    */
    //printf("C\n");
    SDL_DestroyWindow(window);
    //printf("D\n");
    IMG_Quit();
    //printf("E\n");
    TTF_Quit();
    //printf("F\n");
    SDL_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    //printf("G\n");
    fclose(pCharacter_file);
    fclose(pScript_file);
    fclose(pPlayer_sav_file);
    return 0;
}
