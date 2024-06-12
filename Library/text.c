#include "text.h"

void chooseTextColor(uint8_t text_color, SDL_Color* titleTextColor)
{
    if(text_color){
        titleTextColor->r = 255;
        titleTextColor->g = 255;
        titleTextColor->b = 255;
    }else{
        titleTextColor->r = 0;
        titleTextColor->g = 0;
        titleTextColor->b = 0;
    }
}

void displayTextWithShadow(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move, char* option)
{
    // option = "LEFT, "CENTER" or "RIGHT"
    SDL_Color shadowColor = {0, 0, 0};
    displayText(renderer, font, text, shadowColor, x+move, y+move, option);
    displayText(renderer, font, text, titleTextColor, x, y. option);
}

void displayText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, char* option) 
{
    // option = "LEFT, "CENTER" or "RIGHT"
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text, titleTextColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度

    if(option == "CENTER")
    {
        x = x - text_width / 2;
    }
    else if(option == "RIGHT")
    {
        x = x - text_width;
    }
    
    SDL_Rect renderQuad = { x, y, text_width, text_height };  // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作

    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
}