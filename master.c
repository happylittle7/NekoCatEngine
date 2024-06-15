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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // 设置混合模式
    RenderResources resources;
    initRenderResources(&resources);
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
    TTF_Font* font = TTF_OpenFont("./Assets/Font/Cubic_11_1.100_R.ttf", 24); ///這裡的路徑之後會有變化
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
    ////音樂處理
    if (!init_music()) 
    {
        printf("Failed to initialize!\n");
        return 1;
    }
    Mix_Music* bgm = NULL;
    //my_RenderPresent(renderer,&resources,2);
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
                        DisplayTheExpression(renderer, &resources, Character, command, mood);
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
