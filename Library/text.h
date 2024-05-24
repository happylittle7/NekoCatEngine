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