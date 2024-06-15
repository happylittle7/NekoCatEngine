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
//int32_t w_w = 640;
//int32_t w_h = 480;
static int32_t text_start_x = 50;
static int32_t text_start_y = 370;
static int32_t dialogBox_start_x = 0;
static int32_t dialogBox_start_y = 350;
static uint8_t text_color = 0;
static int32_t character_start_x = 50;
static int32_t character_start_y = 320;
static float width_ratio;
static float height_ratio;
typedef struct 
{
    SDL_Texture* text_texture;
    SDL_Rect text_rect;
    SDL_Texture* IMG_texture;
    SDL_Rect IMG_rect;
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
void change_ratio();
int32_t handleButtonEvents(SDL_Event* e, RenderResources* resources) ;
void initButton(SDL_Renderer* renderer, TTF_Font* font, Button* button, SDL_Rect rect, SDL_Color color, const char* text);
int32_t checkLeaveButton(SDL_Event* e, Button* leave_button);
void renderButton(SDL_Renderer* renderer, Button* button);
void initRenderResources(RenderResources* resources);
void freeRenderResources(RenderResources* resources);
void dialogText(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w);
void dialogText_2(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w);
void SetDialogBox(SDL_Renderer* renderer , RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha);
void SetExpression(SDL_Renderer* renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha);
void SetCharacterImg(SDL_Renderer* renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, int32_t idx, uint8_t alpha);
void displayIMG(SDL_Renderer* renderer, RenderResources *resources, toml_table_t *Character, char* directory, const char* Character_id, const char* mood, int32_t idx, int32_t number);
void clearAndRender(RenderResources *resources, SDL_Texture *backgroundTexture );
void DisplayTheExpression(SDL_Renderer* renderer, RenderResources *resources, toml_table_t *Character, char* directory, const char* command, const char* mood);
void my_RenderPresent(SDL_Renderer* renderer, RenderResources* resources, int32_t now_state);
int32_t IsOverGap(toml_table_t *Player_Variable, toml_array_t* variable_array, toml_array_t* mode_array, toml_array_t* gap_array);
int32_t init_music();
void initLeaveButton(RenderResources* resources, SDL_Renderer* renderer, const char* text, TTF_Font* font);
void MakeOption(RenderResources* resources, SDL_Renderer* renderer, const char* optionContent_id, TTF_Font* font, int32_t index, int32_t totalOptions);
//void adjustToFullscreen(RenderResources* resources, int32_t window_width, int32_t window_height, int32_t fullscreen_width, int32_t fullscreen_height);