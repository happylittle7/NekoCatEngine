#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>

int main(int argc, char *argv[]){

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *screenSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    // Create texture
    SDL_Texture *red_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    SDL_Texture *blue_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    
    // Draw to texture
    SDL_SetRenderTarget(renderer, red_texture);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, blue_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, NULL);
     
    // Show textures 
    SDL_RenderCopy(renderer, red_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    SDL_RenderCopy(renderer, blue_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    return 0;
}