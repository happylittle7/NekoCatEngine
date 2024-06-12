#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "./Core/openning.h"

int main(int32_t argc, char* argv[]) 
{
    // Initialize data and path
    openningData data;
    openningPath path;
    openningDataInit(&data); // Initialize pre-defined data
    strcpy(data.title, "貓貓の引擎");
    strcpy(path.backgroundPath, "./Assets/image/openning.png");
    strcpy(path.fontPath, "./Assets/font/Cubic_11_1.100_R.ttf");
    strcpy(path.blackButtonPath, "./Assets/image/black_button.png");
    strcpy(path.whiteButtonPath, "./Assets/image/white_button.png");

    // Initialize SDL and TTF
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    // Create window
    SDL_Window* window = SDL_CreateWindow("Openning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    if (!window) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN); // Fullscreen

    // open the openning window
    int32_t option = openningMain(&data, &path, window);

    // Quit SDL and TTF
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    printf("option = %d\n", option);
    return option;
}