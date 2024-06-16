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

     // Initialize data and path
    openningData data;
    openningPath path;
    openningDataInit(&data); // Initialize pre-defined data
    strcpy(data.title, "貓貓のengine哈哈屁眼party");
    strcpy(path.backgroundPath, "./Assets/image/start_background.png");
    strcpy(path.fontPath, "./Assets/font/Cubic_11_1.100_R.ttf");
    strcpy(path.blackButtonPath, "./Assets/image/black_button.png");
    strcpy(path.whiteButtonPath, "./Assets/image/white_button.png");

    // Create window
    SDL_Window* window = SDL_CreateWindow("Openning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    if (!window) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN); // Fullscreen

    // open the openning window
    printf("1\n");
    int32_t option = openningMain(&data, &path, window);
    printf("2\n");

    SDL_Surface* loaded_dialog_box = IMG_Load("./Assets/dialog_box.png"); ///這裡的路徑之後會有變化
    if (!loaded_dialog_box) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    int32_t w_w = 640;
    int32_t w_h = 480;
    //int32_t fullscreen_width = 700;
    //int32_t fullscreen_height = 700;
    //width_ratio = (float)fullscreen_width/(float)w_w;
    //height_ratio = (float)fullscreen_height/(float)w_h;
    
    
    //printf("%f %f\n",width_ratio,height_ratio);
    //SDL_Window* window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_w, w_h, SDL_WINDOW_SHOWN);
    
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // 设置混合模式
    
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
    
    
    change_ratio(fullscreen_width,w_w,fullscreen_height,w_h);
    
    RenderResources resources;
    initRenderResources(&resources);
    resources.dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    SetDialogBox(renderer, &resources, dialogBox_start_x, dialogBox_start_y, 640, 130, 255);
    
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("./Assets/font/Cubic_11_1.100_R.ttf", 24); ///這裡的路徑之後會有變化
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
    initLeaveButton(&resources,  renderer, "X", font);
    FILE *pCharacter_file;
    FILE *pScript_file;
    FILE *pPlayer_sav_file;
    FILE *pbackground_file;
    FILE *pmusic_file;
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
    toml_table_t *Character = toml_parse_file(pCharacter_file, errbuf, sizeof(errbuf));
    toml_table_t *Script = toml_parse_file(pScript_file, errbuf, sizeof(errbuf));
    //printf("A\n");
    toml_table_t *Player_V = toml_parse_file(pPlayer_sav_file, errbuf, sizeof(errbuf));
    //printf("B\n");
    toml_table_t *background_table = toml_parse_file(pbackground_file, errbuf, sizeof(errbuf));
    toml_table_t *all_music_table = toml_parse_file(pmusic_file, errbuf, sizeof(errbuf));
    background_table = toml_table_in(background_table,"background");
    toml_table_t *music_table = toml_table_in(all_music_table,"Music");
    toml_table_t *sound_table = toml_table_in(all_music_table,"Sound");
    if (!sound_table) 
    {
        printf("Error parsing player.sav: %s\n", errbuf);
        return 1;
    }
    //printf("3\n");
    toml_array_t *char_list = toml_array_in(Character, "char_list");
    //printf("4\n");
    int32_t char_Size = toml_array_nelem(char_list);
    //printf("5\n");
    toml_table_t *now_script = toml_table_in(Script,"Start");
    //printf("6\n");
    toml_table_t *Player_Variable = toml_table_in(Player_V,"Variable");
    //printf("7\n");
    //printf("A\n");
    if (!Player_Variable) 
    {
        printf("Error parsing player.sav: %s\n", errbuf);
        return 1;
    }
    //printf("B\n");
    
    toml_table_t *Player_Items = toml_table_in(Script,"Items");
    toml_array_t *script_list = toml_array_in(now_script, "script");
    ///此處以下為鍵盤互動與顯示，在此處以下，我先不會讓使用者指定背景
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
    Mix_Music* bgm = NULL;
    
    if (option == 2)
    {
        //printf("A\n");
        now_script = toml_table_in(Player_V,"Other");
        //printf("B\n");
        script_list = toml_array_in(now_script, "script");
        //printf("C\n");
    }
    
    while (!quit) 
    {
        while (SDL_PollEvent(&e) != 0) 
        {
            judge_button = handleButtonEvents( &e,  &resources);
            if (now_state!=3)
            {
                if (checkLeaveButton(&e,resources.leave_button) == 1)
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
            else
            {
                //printf("the_judge_button = %d\n",judge_button);
                if (judge_button==-1)
                {
                    quit = 1;
                }
                else if (judge_button!=-2)
                {
                    option_hit = 1;
                    the_option = judge_button;
                }
            }
        }
        if (quit == 1)
            break;
        printf("%d\n",toml_array_nelem(script_list));
        if (col_in_script != toml_array_nelem(script_list))
        {
            toml_table_t *entry = toml_table_at(script_list, col_in_script);
            if (!entry) continue;
            const char *action = toml_raw_in(entry, "action");
            printf("%s\n",action);
            if (strcmp(action, "\"Dialog\"") == 0)
            {
                const char *command = toml_raw_in(entry, "command");
                const char *text_copy = toml_raw_in(entry, "text");
                char* text = calloc(1024,sizeof(char));
                strcpy(text, text_copy);
                text++;
                text[strlen(text)-1] = 0;
                printf("%s\n",text);
                if (now_state == 0)
                {
                    printf("0\n");
                    dialogText_3(renderer,&resources, font, &text, textColor, &print_text_x, &print_text_y, w_w);
                    now_state = 1;
                    had_hit_left = 0;
                }
                else if (now_state == 1 && had_hit_left==1)
                {
                    now_state = 2;
                    had_hit_left = 0;
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
                    }
                }
                else
                {
                    for (int32_t i=0 ; i<3 ; i++) ///初始化character_IMG_texture不然會出事
                    {
                        resources.character_IMG_texture[i] = NULL;
                    }
                    for (int32_t i = 0 ; i < toml_array_nelem(command_array) ; i++)
                    {
                        //printf("A\n");
                        const char* Character_id = toml_string_at(command_array, i).u.s;
                        const char* mood = toml_string_at(mood_array, i).u.s;
                        //char* Character_id_copy = calloc(1024,sizeof(char));
                        //printf("%s\n",Character_id);
                        toml_table_t* Character_id_table = toml_table_in(Character,Character_id);
                        //printf("B\n");
                        toml_datum_t directory_copy = toml_string_in(Character_id_table, "directory");
                        //printf("%s",directory_copy.u.s);
                        //char* directory = calloc(1024,sizeof(char));
                        //strcpy(directory,directory_copy);
                        //directory++;
                        //directory[strlen(directory)-1] = 0;
                        //printf("directory = %s\n",directory_copy.u.s);
                        displayIMG(renderer, &resources, Character, directory_copy.u.s, Character_id, mood, i, toml_array_nelem(command_array)); 
                    }
                }
                col_in_script++;
            }
            else if (strcmp(action, "\"Jump\"") == 0)
            {
                const char *label_id = toml_raw_in(entry, "label_id");
                
                char *label_id_copy = calloc(1024,sizeof(char));
                strcpy(label_id_copy,label_id);
                label_id_copy++;
                label_id_copy[strlen(label_id_copy)-1] = 0;
                printf("%s\n",label_id_copy);
                printf("A\n");
                toml_array_t* variable_array = toml_array_in(entry, "variable");
                printf("B\n");
                toml_array_t* mode_array = toml_array_in(entry, "mode");
                printf("C\n");
                toml_array_t* gap_array = toml_array_in(entry, "gap");
                printf("D\n");
                toml_datum_t col_idx = toml_int_in(entry, "col_idx");
                if (col_idx.ok==0)
                {
                    printf("NO col_idx\n");
                }
                else
                {
                    printf("Yes col_idx\n");
                    printf("the_id = %ld\n",col_idx.u.i);
                }
                if (variable_array != NULL)
                {
                    //printf("E\n");
                    if (IsOverGap(Player_Variable, variable_array,mode_array,gap_array) == 1)
                    {
                        now_script = toml_table_in(Script,label_id_copy);
                        script_list = toml_array_in(now_script, "script");
                        col_in_script = 0;
                    }
                    else
                    {
                        col_in_script++;
                    }
                }
                else if (col_idx.ok==0)
                {
                    printf("the_id_here_1 = %ld\n",col_idx.u.i);
                    now_script = toml_table_in(Script,label_id_copy);
                    script_list = toml_array_in(now_script, "script");
                    col_in_script = 0;
                }
                else
                {
                    printf("the_id_here_2 = %ld\n",col_idx.u.i);
                    now_script = toml_table_in(Script,label_id_copy);
                    script_list = toml_array_in(now_script, "script");
                    col_in_script = (int32_t)col_idx.u.i;
                    
                }
            }
            else if (strcmp(action, "\"Background\"") == 0)
            {
                //printf("here_4\n");
                const char *background_v = toml_raw_in(entry, "background");
                char* background_v_copy = calloc(1024,sizeof(char));
                strcpy(background_v_copy,background_v);
                background_v_copy++;
                background_v_copy[strlen(background_v_copy)-1] = 0;
                
                toml_datum_t background_path_datum = toml_string_in(background_table, background_v_copy);
                if (!background_path_datum.ok) 
                {
                    printf("Error: 'background_1' not found in 'background' table\n");
                }
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
                printf("Music action\n");
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
                //printf("Sound action\n");
                //printf("A\n");
                const char* sound_id = toml_raw_in(entry, "sound_id");
                //printf("B\n");
                char* sound_id_copy = calloc(1024,sizeof(char));
                //printf("C\n");
                //printf("%s\n",sound_id);
                strcpy(sound_id_copy,sound_id);
                //printf("D\n");
                sound_id_copy++;
                sound_id_copy[strlen(sound_id_copy)-1] = 0;
                //printf("%s\n",sound_id_copy);
                toml_datum_t sound_path_datum = toml_string_in(sound_table, sound_id_copy);
                char full_path[512] = {0};
                snprintf(full_path, sizeof(full_path), "./Assets/sound/%s", sound_path_datum.u.s);
                
                Mix_Chunk* sound = load_sound(full_path);
                if (sound!=NULL)
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
                toml_array_t* optionContent = toml_array_in(entry, "optionContent");
                //printf("%d\n",toml_array_nelem(optionContent));
                toml_array_t* optionJump = toml_array_in(entry, "optionJump");
                printf("%d\n",option_hit);
                if (option_hit == 0)
                {
                    if (optionCount.u.i == 0)
                    {
                        for (int32_t i=0 ; i<3 ; i++) ///初始化character_IMG_texture不然會出事
                        {
                            resources.now_option_button[i] = NULL;
                        }
                    }
                    else
                    {
                        for (int32_t i=0 ; i<3 ; i++) ///初始化character_IMG_texture不然會出事
                        {
                            resources.now_option_button[i] = NULL;
                        }
                        for (int32_t i = 0 ; i < toml_array_nelem(optionContent) ; i++)
                        {
                            //printf("A\n");
                            const char* optionContent_id = toml_string_at(optionContent, i).u.s;
                            MakeOption(&resources, renderer, optionContent_id, font, i, optionCount.u.i); 
                        }
                    }
                }
                else
                {
                    
                    //judge_button
                    //printf("the_option = %d\n",the_option);
                    const char* optionJump_id = toml_string_at(optionJump, the_option).u.s;
                    //printf("%s\n",optionJump_id);
                    now_script = toml_table_in(Script,optionJump_id);
                    script_list = toml_array_in(now_script, "script");
                    now_state = 5; //按完按鈕後的clear
                    option_hit = 0;
                    col_in_script = 0;
                    had_hit_left = 0;
                    for (int32_t i=0 ; i<3 ; i++) ///初始化character_IMG_texture不然會出事
                    {
                        resources.now_option_button[i] = NULL;
                    }
                    //SDL_RenderClear(renderer);
                    //clearAndRender( &resources,  resources.background_texture );
                }
            }
            else if(strcmp(action, "\"Modify variable\"") == 0)
            {
                //printf("stay_here_1\n");
                const char* variable_name = toml_raw_in(entry, "variable_name");
                char* variable_name_copy = calloc(1024,sizeof(char));
                strcpy(variable_name_copy,variable_name);
                variable_name_copy++;
                variable_name_copy[strlen(variable_name_copy)-1] = 0;
                toml_datum_t value = toml_string_in(entry, "value");
                //toml_table_t* Modify_table = toml_table_in(Player_Variable,"Variable");
                //printf("A\n");
                //printf("%s\n",value.u.s);
                Modify_Variable(pPlayer_sav_file, Player_Variable, variable_name_copy, value.u.s);
                col_in_script++;
            }
            else if(strcmp(action, "\"Give item\"") == 0)
            {
                
                //printf("stay_here_2\n");
                const char* item_id = toml_raw_in(entry, "item_id");
                char* item_id_copy = calloc(1024,sizeof(char));
                strcpy(item_id_copy,item_id);
                item_id_copy++;
                item_id_copy[strlen(item_id_copy)-1] = 0;
                toml_datum_t value = toml_string_in(entry, "count");
                toml_table_t *Player_Item = toml_table_in(Player_V,"Items");
                //toml_table_t* Modify_table = toml_table_in(Player_Variable,"Variable");
                //printf("A\n");
                //printf("%s\n",value.u.s);
                Modify_Variable(pPlayer_sav_file, Player_Item, item_id_copy, value.u.s);
                col_in_script++;
            }
            //else if (strcmp(action, "\"Music\"") == 0)
            //adjustToFullscreen(&resources, w_w, w_h, fullscreen_width, fullscreen_height);
            my_RenderPresent(renderer,&resources,now_state);
            if(now_state == 2) //進入下一個此場景的action
            {
                col_in_script++;
                now_state = 0;
                had_hit_left = 0;
            }
            else if (now_state == 5) //進入下一個此場景
            {
                now_state = 0;
                had_hit_left = 0;
            }
        }
    }
    //printf("A\n");
    TTF_CloseFont(font);
    freeRenderResources(&resources);
    SDL_DestroyRenderer(renderer);  //這行因為未知原因若不註解掉會有機率錯，但走到這行程式必定結束，資源必定會釋放，所以我註解掉了
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    fclose(pCharacter_file);
    fclose(pScript_file);
    fclose(pPlayer_sav_file);
    return 0;
}