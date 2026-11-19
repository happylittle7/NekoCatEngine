#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "dialog.h"
static int32_t choose_item = -1;
static int32_t now_own_item = -1;

#pragma once

typedef struct _backpackItem
{
    int32_t status; // 0: nothing, 1: obtained, 2: used
    char *name;
    char *description;
    char *image_path;
} backpackItem;

typedef struct _backpackData
{
    double backpack_item_count;
    double backpack_normal_block_width;
    double backpack_normal_block_height;
    double backpack_normal_block_gap;
    double backpack_choose_item_width;
    double backpack_xy[6][2];
    double choose_item_xy[3][2];
    double backpack_choose_item_height[3];
    // double choose_item_xy[3][2];
} backpackData;

typedef struct _backpackPath
{
    // item 的圖片路徑必須在 backpackItem 中設定, 並非在此設定
    char *background_path;
    char *font_path;
    char *black_block_path;
    char *white_edge_black_block_path;
} backpackPath;

typedef struct
{
    SDL_Texture *background_texture;
    SDL_Texture *dialog_box_texture;
    SDL_Rect dialog_box_renderQuad;
    SDL_Texture *text_texture;
    SDL_Rect text_renderQuad;
    SDL_Texture *expression_texture;
    SDL_Rect expression_renderQuad;
    SDL_Texture **character_IMG_texture;
    SDL_Rect **character_IMG_renderQuads;
    SDL_Texture *backpack_block_texture;         // 一般黑背包格子
    SDL_Texture *white_edge_black_block_texture; // 白邊黑背包格子
    SDL_Rect **backpack_block_renderQuads;       // 六個邊框的位置
    SDL_Texture **item_texture;                  // 物品圖片
    SDL_Rect **item_renderQuads;                 // 物品圖片位置
    Button **now_option_button;
    Button *leave_button;
} backpackResources;

void myDisplayText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color titleTextColor, int32_t x, int32_t y, char *option_x, char *option_y);

void myDisplayTextWithShadow(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move, char *option_x, char *option_y);

void backpackDataInit(backpackData *backpack_data);

int32_t backpackMain(int32_t hit, backpackData *data, int32_t *status, char **name, char **description, char **image_path, backpackPath *path, backpackResources *resources, SDL_Renderer *backpack_renderer, TTF_Font *font, SDL_Window *window);

int32_t backpackItemInit(backpackItem **items, int32_t *status, char **name, char **description, char **image_path);

void my_RenderPresentForBackpack(SDL_Renderer *renderer, backpackResources *resources, int32_t now_state, backpackData *data, backpackPath *path, int32_t *status, char **name, char **description, char **image_path, TTF_Font *font, SDL_Window *window);

void initBackpackResources(backpackResources* resources);

void freeBackpackResources(backpackResources* resources);

void multipleLineDialogText(SDL_Renderer* renderer, backpackResources *resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w);

uint8_t myjudgeButtonPressed(int32_t x, int32_t y, int32_t button_x, int32_t button_y, int32_t normal_button_width, int32_t normal_button_height, char *option_x, char *option_y);
