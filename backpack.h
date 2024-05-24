#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

static SDL_Texture* backpackWhiteButtonTexture;
static SDL_Texture* backpackBlackButtonTexture;
static SDL_Texture* whiteBackbackTexture;
static SDL_Texture* blackBackbackTexture;

int32_t loadBackpackImages(char* backpackWhiteButtonTexturePath, char* backpackBlackButtonTexturePath, char* whiteBackbackTexturePath, char* blackBackbackTexturePath, SDL_Renderer* renderer);
