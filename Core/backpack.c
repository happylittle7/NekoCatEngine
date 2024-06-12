#include "backpack.h"

int32_t loadBackpackImages(char* backpackWhiteButtonTexturePath, char* backpackBlackButtonTexturePath, char* whiteBackbackTexturePath, char* blackBackbackTexturePath, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(backpackWhiteButtonTexturePath);
    if(!surface){
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }
    backpackWhiteButtonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(backpackBlackButtonTexturePath);
    if(!surface){
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }
    backpackBlackButtonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(whiteBackbackTexturePath);
    if(!surface){
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }
    whiteBackbackTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(blackBackbackTexturePath);
    if(!surface){
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }
    blackBackbackTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return 0;
}