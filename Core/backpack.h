#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#pragma once

typedef struct _backpackItem
{
    uint8_t status; // 0: nothing, 1: obtained, 2: used
    char name[512];
    char description[1024];
    char image_path[1024];
} backpackItem;

typedef struct _backpackData
{
    double backpack_item_count;
    double backpack_normal_block_width;
    double backpack_normal_block_height;
    double backpack_normal_block_gap;
    double backpack_01_x;
    double backpack_01_y;
    double backpack_02_x;
    double backpack_02_y;
    double backpack_03_x;
    double backpack_03_y;
    double backpack_04_x;
    double backpack_04_y;
    double backpack_05_x;
    double backpack_05_y;
    double backpack_06_x;
    double backpack_06_y;
} backpackData;

typedef struct _backpackPath
{
    // item 的圖片路徑必須在 backpackItem 中設定, 並非在此設定
    char background_path[1024];
    char font_path[1024];
    char black_block_path[1024];
} backpackPath;

void chooseTextColor(uint8_t text_color, SDL_Color* titleTextColor);

void displayText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, char* option_x, char* option_y);

void displayTextWithShadow(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move, char* option_x, char* option_y);

void backpackInit(backpackData *backpack_data, backpackItem **backpack_items);

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h, char *option_x, char *option_y);
