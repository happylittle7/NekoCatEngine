#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, tex, NULL, &dst);
}

int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *black_texture = NULL;
    // SDL_Rect black_rect;
    TTF_Font *font = NULL;

    //創建視窗
    //SDL_Window *SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
    //flags: SDL_WindowFlags
    // SDL_WINDOW_SHOWN — 正常生成可視窗口
    // SDL_WINDOW_FULLSCREEN_DESKTOP — 以電腦解析度生成全螢幕
    // SDL_WINDOW_HIDDEN — 視窗不可見
    // SDL_WINDOW_BORDERLESS — 沒有一般程式視窗的介面
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    //創建渲染器
    //SDL_Renderer *SDL_CreateRenderer(SDL_Window *window, int index, Uint32 flags);
    //flags: SDL_RendererFlags
    // SDL_RENDERER_SOFTWARE — 軟體渲染器
    // SDL_RENDERER_ACCELERATED — 硬體加速渲染器
    // SDL_RENDERER_PRESENTVSYNC — 垂直同步
    // SDL_RENDERER_TARGETTEXTURE — 渲染到紋理
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    //載入圖片
    texture = IMG_LoadTexture(renderer, "hello_world.png");
    if(texture == NULL){
        printf("Unable to load image! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    //建立對話框
    //SDL_Texture *SDL_CreateTexture(SDL_Renderer *renderer, Uint32 format, int access, int w, int h); 
    //format: SDL_PixelFormatEnum
    // SDL_PIXELFORMAT_RGBA8888 — 32位元的RGBA色彩
    // SDL_PIXELFORMAT_ARGB8888 — 32位元的ARGB色彩
    // SDL_PIXELFORMAT_RGB888 — 24位元的RGB色彩
    // SDL_PIXELFORMAT_RGB565 — 16位元的RGB色彩
    //access: SDL_TextureAccess
    // SDL_TEXTUREACCESS_STATIC — 不會改變的紋理
    // SDL_TEXTUREACCESS_STREAMING — 需要頻繁更新的紋理
    // SDL_TEXTUREACCESS_TARGET — 可以用來渲染的紋理 
    black_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 100);
    if(black_texture == NULL){
        printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    // black_rect.x = 0;
    // black_rect.y = 350;
    // black_rect.w = 640;
    // black_rect.h = 100;

    //text
    font = TTF_OpenFont("Jersey10-Regular.ttf", 24);
    if(font == NULL){
        printf("Unable to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Surface *text_surface = TTF_RenderText_Solid(font, "Hello, World!", (SDL_Color){255, 255, 255, 255});
    if(text_surface == NULL){
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if(text_texture == NULL){
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // 渲染對話框
    SDL_SetRenderTarget(renderer, black_texture);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, text_texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderCopy(renderer, text_texture, NULL, &(SDL_Rect){0, 500, 640, 100});
    // 清空渲染器

    //渲染對話框 
    // SDL_RenderFillRect(renderer, &black_rect);
    // SDL_SetRenderTarget(renderer, NULL);
    // 打開渲染目
    renderTexture(texture, renderer, 0, 0, 640, 480);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    renderTexture(black_texture, renderer, 0, 350, 640, 100);
    SDL_RenderPresent(renderer);
    

    while(1){
        SDL_Event windowEvent;
        if(SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type){
                break;
            }
            if(SDL_KEYDOWN == windowEvent.type){
                if(SDLK_ESCAPE == windowEvent.key.keysym.sym){
                    break;
                }
            }
        }
    }
    //destroy window
    SDL_DestroyWindow(window);

    // //hide window
    // SDL_HideWindow(window);

    // //show window
    // SDL_ShowWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}