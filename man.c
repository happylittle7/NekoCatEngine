#include <SDL.h>
#include <stdbool.h>

// 屏幕宽度和高度
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 方块的宽度和高度
const int BLOCK_WIDTH = 100;
const int BLOCK_HEIGHT = 100;

int main(int argc, char* args[]) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 清空渲染器
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // 设置方块的颜色和透明度
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x80); // 红色，透明度为128

    // 定义方块的位置和大小
    SDL_Rect blockRect = {SCREEN_WIDTH / 2 - BLOCK_WIDTH / 2, SCREEN_HEIGHT / 2 - BLOCK_HEIGHT / 2, BLOCK_WIDTH, BLOCK_HEIGHT};

    // 在渲染器上绘制方块
    SDL_RenderFillRect(renderer, &blockRect);

    // 更新屏幕
    SDL_RenderPresent(renderer);

    // 等待窗口关闭
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // 释放资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


//gcc main.c -o main `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf