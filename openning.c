/*
開始畫面


*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "text.h"

#define WHITE 255, 255, 255
#define BLACK 0, 0, 0

static uint8_t title_text_color = 1;
static int32_t title_x = 120;
static int32_t title_y = 100;

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, tex, NULL, &dst);
}

int main(int32_t argc, char* argv[]) 
{
    SDL_Init(SDL_INIT_VIDEO);  // 初始化 SDL2
    TTF_Init();  // 初始化 SDL_ttf
    // char* basePath = SDL_GetBasePath();
    // char* imagePath = (char*)calloc(1, 255);
    // snprintf(imagePath, sizeof(imagePath), "%s%s", basePath, "hello_world.png");
    // printf("imagePath: %s\n", imagePath);
    SDL_Surface* loadedSurface = IMG_Load("./image/openning.png");
    if (!loadedSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    // SDL_Surface* loaded_dialog_box = IMG_Load("./image/dialog_box.png");
    // if (!loaded_dialog_box) 
    // {
    //     printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    //     return -1;
    // }
    SDL_Window* window = SDL_CreateWindow("Openning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    // SDL_Texture* dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);

    // printf("dialog_box : w=%d h=%d\n",loaded_dialog_box->w,loaded_dialog_box->h);
    printf("backgrond : w=%d h=%d\n",loadedSurface->w,loadedSurface->h);
    SDL_FreeSurface(loadedSurface);
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* title_font = TTF_OpenFont("./font/ChakraPetch-Bold.ttf", 24);
    if (!title_font) 
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const char *title = "NekoCat Engine";
 
    SDL_Event e;
    int quit = 0;
    int step = 0;
 
    while (!quit) 
    {
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT) 
            {
                quit = 1;
                break;
            }

            //渲染背景
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景
            SDL_Color titleTextColor;
            chooseTextColor(title_text_color, &titleTextColor);

            //渲染標題字
            title_font = TTF_OpenFont("./font/ChakraPetch-Bold.ttf", 55);
            displayTextWithShadow(renderer, title_font, title, titleTextColor, title_x, title_y, 5);
            SDL_RenderPresent(renderer);
            // renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);
    
            // if (step == 0) 
            // {
            //     char new_text[1024]={0};
            //     for (int32_t i=0 ; i < strlen(firstText) ; i++)
            //     {
            //         new_text[i]=firstText[i];
            //         // displayText(renderer, font, new_text, titleTextColor, text_start_x, text_start_y);
            //         SDL_RenderPresent(renderer);
            //         SDL_Delay(200);  // Display for 2 seconds
            //     }
            //     SDL_Delay(2000);  // Display for 2 seconds
            //     step = 1;
                
            // }
            // else if (step == 1)
            // {
            //     SDL_RenderClear(renderer);
            //     SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 重新渲染背景
            //     // renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);
            //     SDL_RenderPresent(renderer);
            //     SDL_Delay(1000);  // Clear for 1 second
            //     step = 2;
            // }
            // else if (step == 2)
            // {
            //     char new_text[1024]={0};
            //     for (int32_t i=0 ; i < strlen(secondText) ; i++)
            //     {
            //         new_text[i] = secondText[i];
            //         // displayText(renderer, font, new_text, titleTextColor, text_start_x, text_start_y);
            //         SDL_RenderPresent(renderer);
            //         SDL_Delay(200);  // Display for 2 seconds
            //     }
            //     SDL_RenderPresent(renderer);
            //     step = 3;
            //     //SDL_Delay(2000);  // Display for 2 seconds
            // }
        }
    }
 
    TTF_CloseFont(title_font);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}