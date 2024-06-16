#include "toml.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

#pragma once
typedef struct
{
    SDL_Rect rect;
    SDL_Color color;
    const char *text;
    TTF_Font *font;
    SDL_Texture *texture;
} Button;
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
    SDL_Texture *backpack_block_texture; // 一般黑背包格子
    SDL_Texture *white_edge_black_block_texture; // 白邊黑背包格子
    SDL_Rect **backpack_block_renderQuads; // 六個邊框的位置
    SDL_Texture **item_texture; // 物品圖片
    SDL_Rect **item_renderQuads; // 物品圖片位置
    Button **now_option_button;
    Button *leave_button;
} RenderResources;

void initButton(SDL_Renderer *renderer, TTF_Font *font, Button *button, SDL_Rect rect, SDL_Color color, const char *text);
int32_t checkLeaveButton(SDL_Event *e, Button *leave_button);
int32_t handleButtonEvents(SDL_Event *e, RenderResources *resources, int32_t num_option_buttons);
void renderButton(SDL_Renderer *renderer, Button *button);
int32_t isButtonClicked(Button button, int32_t x, int32_t y);
void initRenderResources(RenderResources *resources);
void freeRenderResources(RenderResources *resources);
void displayText(SDL_Renderer *renderer, RenderResources *resources, TTF_Font *font, char **text, SDL_Color textColor, int32_t *x, int32_t *y, int32_t max_w);
void displayText_2(SDL_Renderer *renderer, RenderResources *resources, TTF_Font *font, char **text, SDL_Color textColor, int32_t *x, int32_t *y, int32_t max_w);
void displayText_3(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w);
void SetDialogBox(SDL_Renderer *renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha);
void SetExpression(SDL_Renderer *renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha);
void SetCharacterImg(SDL_Renderer *renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, int32_t idx, uint8_t alpha);
void displayIMG(SDL_Renderer *renderer, RenderResources *resources, toml_table_t *Character, const char *Character_id, const char *mood, int32_t idx, int32_t number);
void clearAndRender(RenderResources *resources, SDL_Texture *backgroundTexture);
void DisplayTheExpression(SDL_Renderer *renderer, RenderResources *resources, toml_table_t *Character, const char *command, const char *mood);
void my_RenderPresent(SDL_Renderer *renderer, RenderResources *resources, int32_t now_state);
int32_t IsOverGap(toml_table_t *Player_Variable, toml_array_t *variable_array, toml_array_t *mode_array, toml_array_t *gap_array);
int32_t init_music();