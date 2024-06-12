/*
假設視覺小說類型是戀愛小說
需要支援好感度指數
增加或減少好感度的數值定義在劇本檔中的選項
需要支援玩家背包與跟物品互動的功能
*/
/*
 
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
// #include "final_header"

static int32_t text_start_x=50;
static int32_t text_start_y=370;
static int32_t dialogBox_start_x=0;
static int32_t dialogBox_start_y=350;

// 顯示文本的函數，負責將指定文本渲染到指定位置
void displayText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color textColor, int32_t x, int32_t y) {

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度

    SDL_Rect renderQuad = { x, y, text_width, text_height };  // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作

    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
}
/*
void displayIMG(SDL_Renderer* renderer, SDL_Texture* my_texture, int32_t x, int32_t y) 
{
    //SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    //SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度
 
    SDL_Rect renderQuad = { x, y, text_width, text_height };  // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
 
    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
}
*/
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
    SDL_Surface* loadedSurface = IMG_Load("./image/background_01.png");
    if (!loadedSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Surface* loaded_dialog_box = IMG_Load("./image/dialog_box.png");
    if (!loaded_dialog_box) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_Texture* dialog_box_texture = SDL_CreateTextureFromSurface(renderer, loaded_dialog_box);

    printf("dialog_box : w=%d h=%d\n",loaded_dialog_box->w,loaded_dialog_box->h);
    printf("backgrond : w=%d h=%d\n",loadedSurface->w,loadedSurface->h);
    SDL_FreeSurface(loadedSurface);
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
 
    TTF_Font* font = TTF_OpenFont("./font/NotoSansTC-VariableFont_wght.ttf", 24);
    if (!font) 
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
 
    SDL_Color textColor = {0, 0, 0};

    const char *firstText = "你好!";
    const char *secondText = "我叫陳柏諺, 你這屁眼! What's your name?";
 
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
            }
        

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景
            renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);

            if (step == 0) 
            {
                displayText(renderer, font, firstText, textColor, text_start_x, text_start_y);
                SDL_RenderPresent(renderer);
                SDL_Delay(2000);  // Display for 2 seconds
                step = 1;
            }else if (step == 1)
            {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 重新渲染背景
                renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);
                SDL_RenderPresent(renderer);
                SDL_Delay(1000);  // Clear for 1 second
                step = 2;
            }else if (step == 2){
                displayText(renderer, font, secondText, textColor, text_start_x, text_start_y);
                SDL_RenderPresent(renderer);
                SDL_Delay(2000);  // Display for 2 seconds
            }
        }
    }
 
    TTF_CloseFont(font);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}