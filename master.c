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
#include "Core/openning.h"
#include "Core/dialog.h"
#include "Core/backpack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <curl/curl.h>
/*
#define WHITE 255, 255, 255
#define BLACK 0, 0, 0
static int32_t quit = 0;

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

int main(int32_t argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO); // 初始化 SDL2
    TTF_Init();               // 初始化 SDL_ttf
    IMG_Init(IMG_INIT_PNG);
    char *font_path = "./Assets/font/Cubic_11_1.100_R.ttf";

    // Initialize data and path
    openningData data;
    openningPath path;
    openningDataInit(&data); // Initialize pre-defined data

    FILE *system_config;
    char errbuf2[200];
    system_config = fopen("system.conf", "r");
    if (!system_config)
    {
        printf("Cannot open system.config\n");
    }
    printf("A\n");
    toml_table_t *BIG_system = toml_parse_file(system_config, errbuf2, sizeof(errbuf2));
    printf("B\n");
    toml_table_t *system = toml_table_in(BIG_system, "Global");
    printf("C\n");
    toml_datum_t temp_data = toml_string_in(system, "gamename");
    printf("D\n");

    // strcpy(data.title, "貓貓のengine哈哈屁眼party");
    strcpy(data.title, temp_data.u.s);
    printf("%s\n", data.title);
    strcpy(path.backgroundPath, "./Assets/image/start_background.png");
    strcpy(path.fontPath, font_path);
    strcpy(path.blackButtonPath, "./Assets/image/black_button.png");
    strcpy(path.whiteButtonPath, "./Assets/image/white_button.png");

    // Create window
    SDL_Window *window = SDL_CreateWindow("Openning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); // Fullscreen

    // open the openning window
    printf("1\n");
    int32_t option = openningMain(&data, &path, window);
    printf("2\n");

    SDL_Surface *loaded_dialog_box = IMG_Load("./Assets/dialog_box.png"); /// 這裡的路徑之後會有變化

    if (!loaded_dialog_box)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    int32_t w_w = 640;
    int32_t w_h = 480;
    // int32_t fullscreen_width = 700;
    // int32_t fullscreen_height = 700;
    // width_ratio = (float)fullscreen_width/(float)w_w;
    // height_ratio = (float)fullscreen_height/(float)w_h;

    // printf("%f %f\n",width_ratio,height_ratio);
    // SDL_Window* window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_w, w_h, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // 设置混合模式

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    /*
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetCurrentDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    */
    int32_t fullscreen_width;
    int32_t fullscreen_height;
    SDL_GetWindowSize(window, &fullscreen_width, &fullscreen_height);

    change_ratio(fullscreen_width, w_w, fullscreen_height, w_h);

    RenderResources resources;
    initRenderResources(&resources);
    backpackResources backpack_resources;
    initBackpackResources(&backpack_resources);
    // RenderResources backpack_resources;
    // initRenderResources(&backpack_resources);
    resources.dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    dialogBox_start_x = -33;
    dialogBox_start_y = 307;
    SetDialogBox(renderer, &resources, dialogBox_start_x, dialogBox_start_y, 700, 200, 255);

    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    TTF_Font *font = TTF_OpenFont(font_path, 24); /// 這裡的路徑之後會有變化
    if (!font)
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Color textColor;
    if (text_color)
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
    /// 此處以下開劇本檔
    FILE *pCharacter_file;
    FILE *pScript_file;
    FILE *pPlayer_sav_file;
    FILE *pbackground_file;
    FILE *pmusic_file;
    printf("before INtia\n");
    initLeaveButton(&resources, renderer, "×", font);
    // initLeaveButton_2(&resources, renderer, "×", font);
    printf("after INtia\n");
    font = TTF_OpenFont(font_path, 20);
    initSaveButton(&resources, renderer, "Save", font);
    printf("here\n");
    // initSaveButton_2(&resources, renderer, "Save", font);
    font = TTF_OpenFont(font_path, 24);
    pCharacter_file = fopen("./character.nekocat", "r");
    if (!pCharacter_file)
    {
        printf("Unable to load character!\n");
    }
    pScript_file = fopen("script.nekocat", "r");
    if (!pScript_file)
    {
        printf("Unable to laod script!\n");
    }
    pPlayer_sav_file = fopen("player.sav", "r+");
    if (!pPlayer_sav_file)
    {
        printf("Error!\n");
    }
    pbackground_file = fopen("./assets.nekocat", "r");
    if (!pbackground_file)
    {
        printf("Error!\n");
    }
    pmusic_file = fopen("./assets.nekocat", "r");
    if (!pmusic_file)
    {
        printf("Error!\n");
    }

    char errbuf[200];
    // printf("3\n");
    toml_table_t *Character = toml_parse_file(pCharacter_file, errbuf, sizeof(errbuf));
    // printf("4\n");
    toml_table_t *Script = toml_parse_file(pScript_file, errbuf, sizeof(errbuf));
    // printf("A\n");
    toml_table_t *Player_V = toml_parse_file(pPlayer_sav_file, errbuf, sizeof(errbuf));
    // printf("B\n");
    toml_table_t *background_table = toml_parse_file(pbackground_file, errbuf, sizeof(errbuf));
    // printf("C\n");
    toml_table_t *all_music_table = toml_parse_file(pmusic_file, errbuf, sizeof(errbuf));
    // printf("D\n");
    background_table = toml_table_in(background_table, "background");
    // printf("E\n");
    toml_table_t *music_table = toml_table_in(all_music_table, "Music");
    // printf("F\n");
    toml_table_t *sound_table = toml_table_in(all_music_table, "Sound");
    
    // printf("G\n");
    if (!sound_table)
    {
        printf("Error parsing player.sav: %s\n", errbuf);
        return 1;
    }
    printf("3\n");
    toml_array_t *char_list = toml_array_in(Character, "char_list");
    printf("4\n");
    int32_t char_Size = toml_array_nelem(char_list);
    printf("5\n");
    toml_table_t *now_script = toml_table_in(Script, "Start");
    printf("6\n");
    toml_table_t *Player_Variable = toml_table_in(Player_V, "Variable");
    printf("7\n");
    // printf("A\n");
    if (!Player_Variable)
    {
        printf("Error parsing player.sav: %s\n", errbuf);
        return 1;
    }
    // printf("B\n");

    toml_table_t *Player_Items = toml_table_in(Script, "Item");
    printf("G\n");
    toml_array_t *script_list = toml_array_in(now_script, "script");
    printf("F\n");
    /// 此處以下為鍵盤互動與顯示，在此處以下，我先不會讓使用者指定背景
    int32_t now_state = 0;
    int32_t col_in_script = 0;
    char *pr_text = NULL;
    SDL_Event e;
    int32_t print_text_x = text_start_x;
    int32_t print_text_y = text_start_y;
    int32_t had_hit_left = 0;
    int32_t option_hit = 0;
    int32_t judge_button = -2;
    int32_t the_option = 0;
    ////音樂處理

    if (!init_music())
    {
        printf("Failed to initialize!\n");
        return 1;
    }
    Mix_Music *bgm = NULL;

    if (option == 2)
    {
        printf("A\n");
        now_script = toml_table_in(Player_V, "Other");
        printf("B\n");
        script_list = toml_array_in(now_script, "script");
        if (script_list == NULL)
        {
            printf("C\n");
            now_script = toml_table_in(Script, "Start");
            script_list = toml_array_in(now_script, "script");
        }
        printf("D\n");
    }

    char save_Background[1024] = {0};
    char save_Music[1024] = {0};
    SetCharacterInfo save_Character;
    // save_Character.number = 0;
    // save_Character.command_list = NULL;
    // save_Character.mood_list = NULL;
    printf("4\n");
    char save_Jump[1024] = {0};
    int32_t save_col_idx = 0;
    strcpy(save_Jump, "Start");
    printf("3\n");
    if (option == 1 || option == 2 || option == 3)
    {
        while (!quit)
        {
            had_hit_left = 0;
            while (SDL_PollEvent(&e) != 0)
            {
                judge_button = handleButtonEvents(&e, &resources);
                if (now_state != 3)
                {
                    if (checkLeaveButton(&e, resources.leave_button) == 1)
                    {
                        quit = 1;
                    }
                    else if (checkSaveButton(&e, resources.save_button) == 1)
                    {
                        printf("here need save\n");
                        SaveIt(pPlayer_sav_file, save_Background, save_Music, save_Character, save_Jump, save_col_idx);
                    }
                    else if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        if (e.button.button == SDL_BUTTON_LEFT)
                        {
                            had_hit_left = 1;
                        }
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        if (e.key.keysym.sym == SDLK_e)
                        {

                            if (now_state != 4)
                            {
                                now_state = 4;
                            }
                            else if (now_state == 4)
                            {
                                SDL_RenderClear(renderer);
                                clearAndRender(&resources, resources.background_texture);
                                now_state = 0;
                                had_hit_left = 0;
                            }
                            // printf("now_state = %d\n",now_state);
                        }
                    }
                }
                else
                {
                    // printf("the_judge_button = %d\n",judge_button);
                    if (judge_button == -1)
                    {
                        quit = 1;
                    }
                    else if (judge_button != -2)
                    {
                        option_hit = 1;
                        the_option = judge_button;
                    }
                }
                // else if (e.type == SDL_KEYDOWN)
                // {
                //     if (e.key.keysym.sym == SDLK_e)
                //     {

                //         if(now_state != 4){
                //             now_state = 4;
                //         }
                //         else if(now_state == 4){
                //             SDL_RenderClear(renderer);
                //             clearAndRender( &resources,  resources.background_texture );
                //             now_state = 0;
                //             had_hit_left = 0;
                //         }
                //         // printf("now_state = %d\n",now_state);
                //     }
                // }
            }
            if (quit == 1)
                break;
            if (now_state == 4)
            {
                // items data
                int32_t *status;
                char **name = calloc(6, sizeof(char *));
                char **description = calloc(6, sizeof(char *));
                char **image_path = calloc(6, sizeof(char *));

                // items initialization
                status = calloc(6, sizeof(int32_t));
                for (int32_t i = 0; i < 6; i++)
                {
                    status[i] = 0;
                    name[i] = calloc(1024, sizeof(char));
                    description[i] = calloc(1024, sizeof(char));
                    image_path[i] = calloc(512, sizeof(char));
                }

                // parsing item.nekocat
                FILE *pItem = fopen("item.nekocat", "r");
                if (!pItem)
                {
                    printf("Error: Unable to load item.nekocat\n");
                }
                toml_table_t *item_table = toml_parse_file(pItem, errbuf, sizeof(errbuf));
                if (!item_table)
                {
                    printf("Error: Unable to parse item.nekocat\n");
                }
                for (int i = 0;; i++)
                {
                    const char *key = toml_key_in(item_table, i);
                    if (!key)
                        break;
                    status[i] = 1;
                    // printf("key %d: %s\n", i, key);
                    toml_table_t *item = toml_table_in(item_table, key);
                    if (!item)
                    {
                        printf("Error: Unable to parse item %s\n", key);
                        continue;
                    }
                    const char *toml_name = toml_string_in(item, "name").u.s;
                    const char *toml_description = toml_string_in(item, "description").u.s;
                    const char *toml_icon_path = toml_string_in(item, "icon_path").u.s;
                    strcpy(name[i], toml_name);
                    strcpy(description[i], toml_description);
                    strcpy(image_path[i], toml_icon_path);
                    now_own_item = i;
                }
                now_own_item++;
                printf("now_own_item = %d\n", now_own_item);
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
                backpack_items = calloc(6, sizeof(backpackItem));
                for (int32_t i = 0; i < 6; i++)
                {
                    backpack_items[i].name = calloc(1024, sizeof(char));
                    backpack_items[i].description = calloc(1024, sizeof(char));
                    backpack_items[i].image_path = calloc(512, sizeof(char));
                }
                // Initialize backpack_items

                int32_t check = -10;

                if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    return -1;
                }
                else
                {
                    printf("SDL_image initialize success!\n");
                }
                // printf("test 08-1\n");
                SDL_Surface *background_surface = IMG_Load("./Assets/background/background_1.png");
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
                SDL_Surface *backpack_block_surface = IMG_Load("./Assets/image/black_button.png");
                if (!backpack_block_surface)
                {
                    printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                    SDL_DestroyRenderer(renderer);
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
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return -1;
                }
                backpack_resources.white_edge_black_block_texture = SDL_CreateTextureFromSurface(renderer, white_edge_black_block_surface);
                SDL_SetTextureAlphaMod(backpack_resources.white_edge_black_block_texture, 200); // 調整透明度
                SDL_FreeSurface(white_edge_black_block_surface);
                // printf("test 08-7\n");

                // 测试加载图像
                // SDL_Surface *temp_surface = IMG_Load("./item.nekocat/eraser.png");
                // if (!temp_surface) {
                //     printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                //     IMG_Quit();
                //     return -1;
                // }
                // SDL_FreeSurface(temp_surface);

                // printf("test 07\n");

                // Backpack main
                // RenderResources backpack_resources;
                // initRenderResources(&backpack_resources);

                // printf("test 08\n");
                // printf("test 08-6\n");

                if (backpack_resources.item_texture == NULL)
                {
                    backpack_resources.item_texture = (SDL_Texture **)malloc(6 * sizeof(SDL_Texture *));
                    for (int i = 0; i < 6; ++i)
                    {
                        backpack_resources.item_texture[i] = NULL;                                     // 分配内存
                        backpack_resources.item_renderQuads[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect)); // 分配内存
                    }
                }
                for (int32_t i = 0; i < 6; i++)
                {
                    // printf("items[%d].status = %d\n", i, status[i]);
                    if (status[i] == 0)
                    {
                        continue;
                    }
                    char *temp;
                    temp = (char *)malloc(sizeof(char) * 100);
                    snprintf(temp, 100, "./Assets/item/%s", image_path[i]);
                    // printf("temp = %s\n", temp);
                    // printf("test 08-6-1\n");
                    SDL_Surface *item_surface = IMG_Load(temp);
                    // SDL_Surface *item_surface = NULL;
                    //  printf("test 08-6-2\n");
                    if (!item_surface)
                    {
                        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return -1;
                    }
                    // printf("test 08-6-3\n");

                    SDL_Texture *temp_tex = SDL_CreateTextureFromSurface(renderer, item_surface);
                    // printf("test 08-6-4\n");
                    if (backpack_resources.item_texture == NULL)
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

                int32_t result = backpackMain(had_hit_left, &backpack_data, status, name, description, image_path, &backpack_path, &backpack_resources, renderer, font, window);

                my_RenderPresentForBackpack(renderer, &backpack_resources, now_state, &backpack_data, &backpack_path, status, name, description, image_path, font, window);
                fclose(pItem);
                now_own_item = -1;
                continue;
            }
            choose_item = -1;
            if (script_list == NULL)
            {
                printf("script_list is NULL\n");
            }
            printf("len = %d\n", toml_array_nelem(script_list));
            
            if (col_in_script != toml_array_nelem(script_list))
            {
                fseek(pPlayer_sav_file,0,SEEK_SET);
                Player_V = toml_parse_file(pPlayer_sav_file, errbuf, sizeof(errbuf));
                save_col_idx = col_in_script;
                toml_table_t *entry = toml_table_at(script_list, col_in_script);
                if(entry==NULL)
                {
                    printf("entry is NULL\n");
                }
                
                if (!entry)
                    continue;
                const char *action = toml_raw_in(entry, "action");
                if(entry==NULL)
                {
                    printf("entry is NULL\n");
                }
                
                printf("%s\n",action);
                if (strcmp(action, "\"Dialog\"") == 0)
                {
                    const char *command = toml_raw_in(entry, "command");
                    const char *text_copy = toml_raw_in(entry, "text");
                    const char *mood = toml_raw_in(entry, "mood");
                    char *text = calloc(1024, sizeof(char));
                    strcpy(text, text_copy);
                    text++;
                    text[strlen(text) - 1] = 0;
                    printf("%s\n", text);
                    if (strcmp(command, "\"no name\"") == 0 || mood == NULL)
                    {
                        resources.name_box_texture = NULL;
                        if (now_state == 0)
                        {
                            // printf("0\n");
                            dialogText_3(renderer, &resources, font, &text, textColor, &print_text_x, &print_text_y, w_w);
                            now_state = 1;
                            had_hit_left = 0;
                        }
                        else if (now_state == 1 && had_hit_left == 1)
                        {
                            now_state = 2;
                            had_hit_left = 0;
                        }
                    }
                    else
                    {

                        char *command_cpy = calloc(1024, sizeof(char));
                        strcpy(command_cpy, command);
                        command_cpy++;
                        command_cpy[strlen(command_cpy) - 1] = 0;
                        toml_table_t *Character_id_table = toml_table_in(Character, command_cpy);
                        // printf("2\n");
                        toml_datum_t directory_copy = toml_string_in(Character_id_table, "directory");
                        toml_datum_t name_copy = toml_string_in(Character_id_table, "name");
                        toml_datum_t color_copy = toml_string_in(Character_id_table, "color");
                        SDL_Color name_color = {0, 0, 0, 255};
                        if (color_copy.ok != 0)
                        {
                            name_color = hexStringToSDL_Color(color_copy.u.s);
                        }
                        char *character_name = calloc(1024, sizeof(char));
                        strcpy(character_name, name_copy.u.s);
                        // char* directory = calloc(1024,sizeof(char));
                        // strcpy(directory,directory_copy);
                        // directory++;
                        // directory[strlen(directory)-1] = 0;
                        int32_t name_x = 50;
                        int32_t name_y = 300;
                        dialogName_3(renderer, &resources, font, &character_name, name_color, &name_x, &name_y, w_w);
                        if (character_name != NULL)
                        {
                            SDL_Surface *backpack_block_surface;
                            if (name_color.r > 200 || name_color.g > 200 || name_color.b > 200)
                            {
                                backpack_block_surface = IMG_Load("./Assets/image/black_button.png");
                            }
                            else
                            {
                                backpack_block_surface = IMG_Load("./Assets/image/white_button.png");
                            }
                            if (!backpack_block_surface)
                            {
                                printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_Quit();
                                return -1;
                            }
                            if (resources.name_box_texture)
                            {
                                SDL_DestroyTexture(resources.name_box_texture);
                            }
                            resources.name_box_texture = SDL_CreateTextureFromSurface(renderer, backpack_block_surface);
                            SDL_SetTextureAlphaMod(resources.name_box_texture, 200); // 調整透明度
                            SDL_FreeSurface(backpack_block_surface);
                        }

                        // SDL_RenderCopy(renderer, name_black_block_texture, NULL, &resources.name_Rect);

                        DisplayTheExpression(renderer, &resources, Character, directory_copy.u.s, command, mood);

                        if (now_state == 0)
                        {
                            // printf("0\n");
                            font = TTF_OpenFont(font_path, 20);
                            dialogText_3(renderer, &resources, font, &text, textColor, &print_text_x, &print_text_y, w_w);
                            now_state = 1;
                            had_hit_left = 0;
                            font = TTF_OpenFont(font_path, 24);
                        }
                        else if (now_state == 1 && had_hit_left == 1)
                        {
                            now_state = 2;
                            had_hit_left = 0;
                            resources.expression_texture = NULL;
                            resources.name_texture = NULL;
                        }
                    }
                    /*
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
                            resources.expression_texture = NULL;
                            if (had_hit_left == 1)
                            {
                                dialogText_2(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                                now_state = 1;
                                had_hit_left = 0;
                            }
                            else
                                dialogText(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                        }
                        else
                        {
                            //printf("1\n");
                            const char *mood = toml_raw_in(entry, "mood");
                            char* command_cpy = calloc(1024,sizeof(char));
                            strcpy(command_cpy,command);
                            command_cpy++;
                            command_cpy[strlen(command_cpy)-1] = 0;
                            toml_table_t* Character_id_table = toml_table_in(Character,command_cpy);
                            //printf("2\n");
                            toml_datum_t directory_copy = toml_string_in(Character_id_table, "directory");
                            //char* directory = calloc(1024,sizeof(char));
                            //strcpy(directory,directory_copy);
                            //directory++;
                            //directory[strlen(directory)-1] = 0;
                            DisplayTheExpression(renderer, &resources, Character, directory_copy.u.s, command, mood);
                            if (had_hit_left == 1)
                            {
                                dialogText_2(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                                now_state = 1;
                                had_hit_left = 0;
                            }
                            else
                                dialogText(renderer,&resources, font, &pr_text, textColor, &print_text_x, &print_text_y, w_w);
                        }
                    }
                    else if (now_state == 1 && had_hit_left == 1) //跳入下一段對話的前置作業
                    {
                        now_state = 2;
                        pr_text = NULL;
                        SDL_RenderClear(renderer);
                        //my_RenderPresent(renderer,&resources,now_state);
                        clearAndRender( &resources,  resources.background_texture );
                        my_RenderPresent(renderer,&resources,now_state);
                        print_text_x = text_start_x;
                        print_text_y = text_start_y;
                        had_hit_left = 0;
                    }
                    else
                    {
                        now_state = 1;
                    }
                    */
                }
                else if (strcmp(action, "\"SetCharacter\"") == 0)
                {
                    toml_datum_t character_number = toml_int_in(entry, "number");
                    toml_array_t *command_array = toml_array_in(entry, "command_list");
                    toml_array_t *mood_array = toml_array_in(entry, "mood_list");

                    if (character_number.u.i == 0)
                    {
                        save_Character.number = character_number.u.i;
                        for (int32_t i = 0; i < 3; i++) /// 初始化character_IMG_texture不然會出事
                        {
                            resources.character_IMG_texture[i] = NULL;
                        }
                    }
                    else
                    {
                        save_Character.number = character_number.u.i;
                        // save_Character.command_list_size = toml_array_nelem(number);
                        // save_Character.mood_list_size = toml_array_nelem(mood_array);
                        save_Character.command_list = (char **)malloc(character_number.u.i * sizeof(char *));
                        save_Character.mood_list = (char **)malloc(character_number.u.i * sizeof(char *));
                        for (int32_t i = 0; i < 3; i++) /// 初始化character_IMG_texture不然會出事
                        {
                            resources.character_IMG_texture[i] = NULL;
                        }
                        for (int32_t i = 0; i < toml_array_nelem(command_array); i++)
                        {
                            // printf("A\n");
                            const char *Character_id = toml_string_at(command_array, i).u.s;
                            save_Character.command_list[i] = strdup(Character_id);

                            const char *mood = toml_string_at(mood_array, i).u.s;
                            save_Character.mood_list[i] = strdup(mood);
                            // char* Character_id_copy = calloc(1024,sizeof(char));
                            // printf("%s\n",Character_id);
                            toml_table_t *Character_id_table = toml_table_in(Character, Character_id);
                            // printf("B\n");
                            toml_datum_t directory_copy = toml_string_in(Character_id_table, "directory");
                            // printf("%s",directory_copy.u.s);
                            // char* directory = calloc(1024,sizeof(char));
                            // strcpy(directory,directory_copy);
                            // directory++;
                            // directory[strlen(directory)-1] = 0;
                            // printf("directory = %s\n",directory_copy.u.s);
                            displayIMG(renderer, &resources, Character, directory_copy.u.s, Character_id, mood, i, toml_array_nelem(command_array));
                        }
                    }
                    col_in_script++;
                }
                else if (strcmp(action, "\"Jump\"") == 0)
                {
                    printf("IN the Jump\n");
                    const char *label_id = toml_raw_in(entry, "label_id");
                    char *label_id_copy = calloc(1024, sizeof(char));
                    strcpy(label_id_copy, label_id);
                    label_id_copy++;
                    label_id_copy[strlen(label_id_copy) - 1] = 0;
                    strcpy(save_Jump, label_id_copy);
                    // printf("%s\n",label_id_copy);
                    // printf("A\n");
                    toml_array_t *variable_array = toml_array_in(entry, "variable");
                    if (variable_array == NULL)
                    {
                        printf("variable_array = NULL\n");
                    }
                    // printf("B\n");
                    toml_array_t *mode_array = toml_array_in(entry, "mode");
                    if (mode_array == NULL)
                    {
                        printf("mode_array = NULL\n");
                    }
                    // printf("C\n");
                    toml_array_t *gap_array = toml_array_in(entry, "gap");
                    if (gap_array == NULL)
                    {
                        printf("gap_array = NULL\n");
                    }
                    // printf("D\n");
                    toml_datum_t col_idx = toml_int_in(entry, "col_idx");
                    if (col_idx.ok == 0)
                    {
                        printf("col_idx = NULL\n");
                    }
                    if (col_idx.ok == 0)
                    {
                        printf("NO col_idx\n");
                    }
                    else
                    {
                        printf("Yes col_idx\n");
                        printf("the_id = %ld\n", col_idx.u.i);
                    }
                    //Player_V = toml_parse_file(pPlayer_sav_file, errbuf, sizeof(errbuf));
                    Player_Variable = toml_table_in(Player_V,"Variable");
                    if (variable_array != NULL)
                    {
                        printf("now_here is %d\n", IsOverGap(Player_Variable, variable_array, mode_array, gap_array));
                        if (IsOverGap(Player_Variable, variable_array, mode_array, gap_array) == 1)
                        {
                            now_script = toml_table_in(Script, label_id_copy);
                            script_list = toml_array_in(now_script, "script");
                            col_in_script = 0;
                        }
                        else
                        {
                            col_in_script++;
                        }
                    }
                    else if (col_idx.ok == 0)
                    {
                        printf("the_id_here_1 = %ld\n", col_idx.u.i);
                        now_script = toml_table_in(Script, label_id_copy);
                        script_list = toml_array_in(now_script, "script");
                        col_in_script = 0;
                    }
                    else
                    {
                        printf("the_id_here_2 = %ld\n", col_idx.u.i);
                        now_script = toml_table_in(Script, label_id_copy);
                        script_list = toml_array_in(now_script, "script");
                        col_in_script = (int32_t)col_idx.u.i;
                    }
                }
                else if (strcmp(action, "\"Background\"") == 0)
                {
                    // printf("here_4\n");
                    const char *background_v = toml_raw_in(entry, "background");
                    printf("%s\n", background_v);
                    char *background_v_copy = calloc(1024, sizeof(char));
                    strcpy(background_v_copy, background_v);
                    background_v_copy++;
                    background_v_copy[strlen(background_v_copy) - 1] = 0;

                    strcpy(save_Background, background_v_copy);
                    toml_datum_t background_path_datum = toml_string_in(background_table, background_v_copy);
                    if (!background_path_datum.ok)
                    {
                        printf("Error: '%s' not found in 'background' table\n", background_v_copy);
                    }
                    char full_path[512] = {0};

                    snprintf(full_path, sizeof(full_path), "./Assets/background/%s", background_path_datum.u.s);
                    SDL_Surface *background_surface = IMG_Load(full_path);
                    if (!background_surface)
                    {
                        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
                        return -1;
                    }
                    if (resources.background_texture)
                    {
                        SDL_DestroyTexture(resources.background_texture);
                    }
                    resources.background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
                    SDL_FreeSurface(background_surface);
                    now_state = 2;
                }
                else if (strcmp(action, "\"Music\"") == 0)
                {
                    printf("Music action\n");
                    const char *music_id = toml_raw_in(entry, "music_id");
                    char *music_id_copy = calloc(1024, sizeof(char));
                    strcpy(music_id_copy, music_id);
                    music_id_copy++;
                    music_id_copy[strlen(music_id_copy) - 1] = 0;
                    strcpy(save_Music, music_id_copy);
                    toml_datum_t music_path_datum = toml_string_in(music_table, music_id_copy);
                    if (bgm != NULL)
                    {
                        Mix_HaltMusic();
                        Mix_FreeMusic(bgm);
                    }
                    if (strcmp(music_id_copy, "close") != 0)
                    {
                        char full_path[512] = {0};

                        snprintf(full_path, sizeof(full_path), "./Assets/music/%s", music_path_datum.u.s);

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
                        else
                        {
                            printf("music OK!\n");
                        }
                    }
                    now_state = 2;
                }
                else if (strcmp(action, "\"Sound\"") == 0)
                {
                    // printf("Sound action\n");
                    // printf("A\n");
                    const char *sound_id = toml_raw_in(entry, "sound_id");
                    // printf("B\n");
                    char *sound_id_copy = calloc(1024, sizeof(char));
                    // printf("C\n");
                    // printf("%s\n",sound_id);
                    strcpy(sound_id_copy, sound_id);
                    // printf("D\n");
                    sound_id_copy++;
                    sound_id_copy[strlen(sound_id_copy) - 1] = 0;
                    // printf("%s\n",sound_id_copy);
                    toml_datum_t sound_path_datum = toml_string_in(sound_table, sound_id_copy);
                    char full_path[512] = {0};
                    snprintf(full_path, sizeof(full_path), "./Assets/sound/%s", sound_path_datum.u.s);

                    Mix_Chunk *sound = load_sound(full_path);
                    if (sound != NULL)
                    {
                        Mix_PlayChannel(-1, sound, 0);
                    }
                    else
                    {
                        printf("no sound\n");
                    }

                    now_state = 2;
                }
                else if (strcmp(action, "\"Option\"") == 0)
                {
                    now_state = 3;
                    toml_datum_t optionCount = toml_int_in(entry, "optionCount");
                    toml_array_t *optionContent = toml_array_in(entry, "optionContent");
                    // printf("%d\n",toml_array_nelem(optionContent));
                    toml_array_t *optionJump = toml_array_in(entry, "optionJump");
                    printf("%d\n", option_hit);
                    if (option_hit == 0)
                    {
                        if (optionCount.u.i == 0)
                        {
                            for (int32_t i = 0; i < 3; i++) /// 初始化character_IMG_texture不然會出事
                            {
                                resources.now_option_button[i] = NULL;
                            }
                        }
                        else
                        {
                            for (int32_t i = 0; i < 3; i++) /// 初始化character_IMG_texture不然會出事
                            {
                                resources.now_option_button[i] = NULL;
                            }
                            for (int32_t i = 0; i < toml_array_nelem(optionContent); i++)
                            {
                                // printf("A\n");
                                const char *optionContent_id = toml_string_at(optionContent, i).u.s;
                                MakeOption(&resources, renderer, optionContent_id, font, i, optionCount.u.i);
                            }
                        }
                    }
                    else
                    {

                        // judge_button
                        // printf("the_option = %d\n",the_option);
                        const char *optionJump_id = toml_string_at(optionJump, the_option).u.s;
                        // printf("%s\n",optionJump_id);
                        strcpy(save_Jump, optionJump_id);
                        now_script = toml_table_in(Script, optionJump_id);
                        script_list = toml_array_in(now_script, "script");
                        now_state = 5; // 按完按鈕後的clear
                        option_hit = 0;
                        col_in_script = 0;
                        had_hit_left = 0;
                        for (int32_t i = 0; i < 3; i++) /// 初始化character_IMG_texture不然會出事
                        {
                            resources.now_option_button[i] = NULL;
                        }
                        // SDL_RenderClear(renderer);
                        // clearAndRender( &resources,  resources.background_texture );
                    }
                }
                else if (strcmp(action, "\"Modify variable\"") == 0)
                {
                    // printf("stay_here_1\n");
                    const char *variable_name = toml_raw_in(entry, "variable_name");
                    char *variable_name_copy = calloc(1024, sizeof(char));
                    strcpy(variable_name_copy, variable_name);
                    variable_name_copy++;
                    variable_name_copy[strlen(variable_name_copy) - 1] = 0;
                    toml_datum_t value = toml_string_in(entry, "value");
                    // toml_table_t* Modify_table = toml_table_in(Player_Variable,"Variable");
                    // printf("A\n");
                    // printf("%s\n",value.u.s);
                    Modify_Variable(pPlayer_sav_file, Player_Variable, variable_name_copy, value.u.s);
                    col_in_script++;
                }
                else if (strcmp(action, "\"Give item\"") == 0)
                {

                    // printf("stay_here_2\n");
                    const char *item_id = toml_raw_in(entry, "item_id");
                    char *item_id_copy = calloc(1024, sizeof(char));
                    strcpy(item_id_copy, item_id);
                    item_id_copy++;
                    item_id_copy[strlen(item_id_copy) - 1] = 0;
                    toml_datum_t value = toml_string_in(entry, "count");
                    if (value.ok == 0)
                    {
                        printf("value not OK\n");
                    }
                    //Player_V = toml_parse_file(pPlayer_sav_file, errbuf, sizeof(errbuf));
                    if (Player_V == NULL)
                    {
                        printf("Player_V is NULL\n");
                    }
                    toml_table_t *Player_Item = toml_table_in(Player_V, "Items");
                    if (Player_Item == NULL)
                    {
                        printf("Player_Item is NULL\n");
                    }
                    // toml_table_t* Modify_table = toml_table_in(Player_Variable,"Variable");
                    // printf("A\n");
                    // printf("%s\n",value.u.s);
                    Modify_Variable(pPlayer_sav_file, Player_Item, item_id_copy, value.u.s);
                    col_in_script++;
                }
                // else if (strcmp(action, "\"Music\"") == 0)
                // adjustToFullscreen(&resources, w_w, w_h, fullscreen_width, fullscreen_height);
                my_RenderPresent(renderer, &resources, now_state);
                if (now_state == 2) // 進入下一個此場景的action
                {
                    col_in_script++;
                    now_state = 0;
                    had_hit_left = 0;
                }
                else if (now_state == 5) // 進入下一個此場景
                {
                    now_state = 0;
                    had_hit_left = 0;
                }
            }
            else
            {
                // TODO: Ending
                quit = 1;
            }
        }
    }

    // printf("A\n");
    TTF_CloseFont(font);
    freeRenderResources(&resources);
    SDL_DestroyRenderer(renderer); // 這行因為未知原因若不註解掉會有機率錯，但走到這行程式必定結束，資源必定會釋放，所以我註解掉了
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    fclose(pCharacter_file);
    fclose(pScript_file);
    //fclose(pPlayer_sav_file);
    return 0;
}