/*
假設視覺小說類型是戀愛小說
需要支援好感度指數
增加或減少好感度的數值定義在劇本檔中的選項
需要支援玩家背包與跟物品互動的功能
*/
/*

*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "./Core/backpack.h"

#define WHITE 255, 255, 255
#define BLACK 0, 0, 0
// #include "final_header"

static int32_t text_start_x = 50;
static int32_t text_start_y = 370;
static int32_t dialogBox_start_x = 0;
static int32_t dialogBox_start_y = 350;
static uint8_t text_color = 0;

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

int main(int32_t argc, char *argv[])
{
    // 初始化
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // 設定連結
    char *background_path = "./Assets/image/background_01.png";
    char *dialog_box_path = "./Assets/image/dialog_box.png";
    char *font_path = "./Assets/font/Cubic_11_1.100_R.ttf";


    // 開啟視窗
    SDL_Window *window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 建立渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 載入背景圖片
    SDL_Surface *loadedSurface = IMG_Load(background_path);
    if (!loadedSurface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    printf("backgrond : w=%d h=%d\n", loadedSurface->w, loadedSurface->h);

    // 載入對話框圖片
    SDL_Surface *loaded_dialog_box = IMG_Load(dialog_box_path);
    if (!loaded_dialog_box)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);
    printf("dialog_box : w=%d h=%d\n", loaded_dialog_box->w, loaded_dialog_box->h);

    // loadBackpackImages("./Assets/image/white_button.png", "./Assets/image/black_button.png", "./Assets/image/white_backpack.png", "./Assets/image/black_backpack.png", renderer);

    // 釋放表面資源
    SDL_FreeSurface(loadedSurface);
    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 載入字體
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    if (!font)
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 選擇字體顏色
    SDL_Color textColor;
    chooseTextColor(text_color, &textColor);

    const char *firstText = "Hello!";
    const char *secondText = "What's your name?";

    SDL_Event e;
    int quit = 0;
    int step = 0;
    int32_t bag_mod_openned = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0 && !quit)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_e:
                    printf("E key pressed\n");
                    bag_mod_openned = !bag_mod_openned;
                    break;
                default:
                    break;
                }
                break;
            }
        }

        if (quit)
        {
            break;
        }

        // 背包模式
        if (bag_mod_openned)
        {
            // 初始化背包
            backpackData backpack_data;
            backpackItem *backpack_items;
            backpackPath backpack_path;

            backpackInit(&backpack_data, &backpack_items);
            strcpy(backpack_path.background_path, background_path);
            strcpy(backpack_path.font_path, font_path);
            strcpy(backpack_path.black_block_path, "./Assets/image/black_block.png");
            strcpy(backpack_path.white_edge_black_block_path, "./Assets/image/white_edge_black_block.png");
            
            // 初始化背包物品
            backpack_items[0].status = 1;
            backpack_items[1].status = 2;
            backpack_items[2].status = 0;
            backpack_items[3].status = 0;
            backpack_items[4].status = 0;
            backpack_items[5].status = 0;
            strcpy(backpack_items[0].name, "Item 1");
            strcpy(backpack_items[1].name, "Item 2");
            strcpy(backpack_items[2].name, "Item 3");
            strcpy(backpack_items[3].name, "Item 4");
            strcpy(backpack_items[4].name, "Item 5");
            strcpy(backpack_items[5].name, "Item 6");
            strcpy(backpack_items[0].description, "Item 1 description");
            strcpy(backpack_items[1].description, "Item 2 description");
            strcpy(backpack_items[2].description, "Item 3 description");
            strcpy(backpack_items[3].description, "Item 4 description");
            strcpy(backpack_items[4].description, "Item 5 description");
            strcpy(backpack_items[5].description, "Item 6 description");
            strcpy(backpack_items[0].image_path, "./Assets/image/item_01.png");
            strcpy(backpack_items[1].image_path, "./Assets/image/item_02.png");
            strcpy(backpack_items[2].image_path, "./Assets/image/item_03.png");
            strcpy(backpack_items[3].image_path, "./Assets/image/item_04.png");
            strcpy(backpack_items[4].image_path, "./Assets/image/item_05.png");
            strcpy(backpack_items[5].image_path, "./Assets/image/item_06.png");

        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // 渲染背景
        renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130, "LEFT", "TOP");

        if (step == 0)
        {
            char new_text[1024] = {0};
            for (int32_t i = 0; i < strlen(firstText); i++)
            {
                new_text[i] = firstText[i];
                myDisplayText(renderer, font, new_text, textColor, text_start_x, text_start_y, "LEFT", "TOP");
                SDL_RenderPresent(renderer);
                SDL_Delay(200); // Display for 2 seconds
            }
            SDL_Delay(2000); // Display for 2 seconds
            step = 1;
        }
        else if (step == 1)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // 重新渲染背景
            renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130, "LEFT", "TOP");
            SDL_RenderPresent(renderer);
            SDL_Delay(1000); // Clear for 1 second
            step = 2;
        }
        else if (step == 2)
        {
            char new_text[1024] = {0};
            for (int32_t i = 0; i < strlen(secondText); i++)
            {
                new_text[i] = secondText[i];
                myDisplayText(renderer, font, new_text, textColor, text_start_x, text_start_y, "LEFT", "TOP");
                SDL_RenderPresent(renderer);
                SDL_Delay(200); // Display for 2 seconds
            }
            SDL_RenderPresent(renderer);
            step = 3;
            // SDL_Delay(2000);  // Display for 2 seconds
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