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
    uint8_t title_text_color = 1;
    int32_t title_x = 120 + 20;
    int32_t title_y = 100;
    int32_t window_width = 640;
    int32_t window_height = 480;
    int32_t start_button_x = 240;
    int32_t start_button_y = 250;
    int32_t aboutus_button_x = start_button_x;
    int32_t aboutus_button_y = start_button_y + 80;
    int32_t button_width = 160;
    int32_t button_height = 60;
    int32_t option = 0; //-1 = Error, 0 = Quit, 1 = start, 2 = aboutus
    const char *title = "NekoCat Engine";//標題

    SDL_Init(SDL_INIT_VIDEO);  // 初始化 SDL2
    TTF_Init();  // 初始化 SDL_ttf

    //開啟視窗
    SDL_Window* window = SDL_CreateWindow("Openning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //開始頁面背景圖片
    SDL_Surface* loadedSurface = IMG_Load("./image/openning.png");
    if (!loadedSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
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
        return -1;
    }

    //字體
    TTF_Font* title_font = TTF_OpenFont("./font/Cubic_11_1.100_R.ttf", 55);
    if (!title_font) 
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //按鈕
    SDL_Surface* blackbuttonSurface = IMG_Load("./image/black_button.png");
    if (!blackbuttonSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture* blackbuttonTexture = SDL_CreateTextureFromSurface(renderer, blackbuttonSurface);
    SDL_SetTextureAlphaMod(blackbuttonTexture, 200); //調整按鈕透明度

    SDL_Surface* whitebuttonSurface = IMG_Load("./image/white_button.png");
    if (!whitebuttonSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture* whitebuttonTexture = SDL_CreateTextureFromSurface(renderer, whitebuttonSurface);
    SDL_SetTextureAlphaMod(whitebuttonTexture, 200); //調整按鈕透明度
 
    SDL_Event e;
    int32_t quit = 0;
    int32_t step = 0;
    int32_t mouse_x = 0, mouse_y = 0;

    while (!quit) 
    {
        int32_t mouse_press = 0;

        while (SDL_PollEvent(&e) != 0 && !quit) 
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    printf("quit\n");
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_press = 1;
                    break;
                default:
                    break;
            }
            SDL_GetMouseState(&mouse_x, &mouse_y); //處理鼠標位置
        }

        //渲染背景
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景

        //渲染標題字
        SDL_Color titleTextColor;
        chooseTextColor(title_text_color, &titleTextColor);
        title_font = TTF_OpenFont("./font/Cubic_11_1.100_R.ttf", 55);
        displayTextWithShadow(renderer, title_font, title, titleTextColor, title_x, title_y, 5);
        // SDL_RenderPresent(renderer);
        // renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);

        //start button
        SDL_Color startTextColor;
        title_font = TTF_OpenFont("./font/Cubic_11_1.100_R.ttf", 30);
        if(mouse_x >= start_button_x && mouse_x <= start_button_x + button_width && mouse_y >= start_button_y && mouse_y <= start_button_y + button_height)
        {
            chooseTextColor(0, &startTextColor);
            renderTexture(whitebuttonTexture, renderer, start_button_x, start_button_y, button_width, button_height);
            displayText(renderer, title_font, "Start", startTextColor, start_button_x + 47, start_button_y + 10);

            if(mouse_press == 1)
            {
                printf("start button is pressed\n");
                quit = 1;
                option = 1;
            }
            // printf("mouse on start button\n");
        }
        else
        {
            chooseTextColor(1, &startTextColor);
            renderTexture(blackbuttonTexture, renderer, start_button_x, start_button_y, button_width, button_height);
            displayText(renderer, title_font, "Start", startTextColor, start_button_x + 47, start_button_y + 10);
        }

        //aboutus button
        SDL_Color aboutusTextColor;
        title_font = TTF_OpenFont("./font/Cubic_11_1.100_R.ttf", 30);
        if(mouse_x >= aboutus_button_x && mouse_x <= aboutus_button_x + button_width && mouse_y >= aboutus_button_y && mouse_y <= aboutus_button_y + button_height)
        {
            chooseTextColor(0, &aboutusTextColor);
            renderTexture(whitebuttonTexture, renderer, aboutus_button_x, aboutus_button_y, button_width, button_height);
            displayText(renderer, title_font, "Aboutus", aboutusTextColor, aboutus_button_x + 27, aboutus_button_y + 10);
            // printf("mouse on aboutus button\n");
            if(mouse_press == 1)
            {
                printf("aboutus button is pressed\n");
                quit = 1;
                option = 2;
            }
        }
        else
        {
            chooseTextColor(1, &aboutusTextColor);
            renderTexture(blackbuttonTexture, renderer, aboutus_button_x, aboutus_button_y, button_width, button_height);
            displayText(renderer, title_font, "Aboutus", aboutusTextColor, aboutus_button_x + 27, aboutus_button_y + 10);
        }

        SDL_RenderPresent(renderer);


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
 
    TTF_CloseFont(title_font);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    printf("option = %d\n", option);
    return option;
}