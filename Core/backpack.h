#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "dialog.h"

#pragma once

typedef struct _backpackItem
{
    int32_t status; // 0: nothing, 1: obtained, 2: used
    char* name;
    char* description;
    char* image_path;
} backpackItem;

typedef struct _backpackData
{
    double backpack_item_count;
    double backpack_normal_block_width;
    double backpack_normal_block_height;
    double backpack_normal_block_gap;
    double backpack_xy[6][2];
} backpackData;

typedef struct _backpackPath
{
    // item 的圖片路徑必須在 backpackItem 中設定, 並非在此設定
    char* background_path;
    char* font_path;
    char* black_block_path;
    char* white_edge_black_block_path;
} backpackPath;

void chooseTextColor(uint8_t text_color, SDL_Color* titleTextColor);

void myDisplayText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, char* option_x, char* option_y);

void myDisplayTextWithShadow(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move, char* option_x, char* option_y);

void backpackDataInit(backpackData *backpack_data);

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h, char *option_x, char *option_y);

int32_t backpackMain(backpackData *data, int32_t *status, char **name, char **description, char **image_path, backpackPath *path, RenderResources* resources, SDL_Renderer *backpack_renderer, TTF_Font* font, SDL_Window *window);

int32_t backpackItemInit(backpackItem **items, int32_t *status, char **name, char **description, char **image_path);

void my_RenderPresentForBackpack(SDL_Renderer *renderer, RenderResources *resources, int32_t now_state, backpackData *data,  int32_t *status, char **name, char **description, char **image_path, TTF_Font *font, SDL_Window *window);