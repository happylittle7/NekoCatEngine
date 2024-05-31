#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

void chooseTextColor(uint8_t text_color, SDL_Color* titleTextColor);

void displayText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y);

void displayTextWithShadow(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move);

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h);

typedef struct _openningData{
    uint8_t title_text_color;  // the color of texts, 0 = black, 1 = white
    double title_x; // title position x
    double title_y; // title position y
    int32_t window_width; // window width
    int32_t window_height; // window height
    double start_button_x; // start button position x
    double start_button_y; // start button position y 
    double aboutus_button_x; // aboutus button position x
    double aboutus_button_y; // aboutus button position y
    double button_width; // button width
    double button_height; // button height
    char title[1024]; // title
} openningData;

typedef struct _openningPath{
    char backgroundPath[1024];
    char fontPath[1024];
    char blackButtonPath[1024];
    char whiteButtonPath[1024];
} openningPath;

void openningDataInit(openningData* data);

int32_t openningMain(openningData* data, openningPath* path, SDL_Window* window);

uint8_t judgeButtonPressed(int32_t x, int32_t y, int32_t button_x, int32_t button_y, int32_t button_width, int32_t button_height);