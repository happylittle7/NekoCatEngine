#include "backpack.h"

void chooseTextColor(uint8_t text_color, SDL_Color *titleTextColor)
{
    if (text_color)
    {
        titleTextColor->r = 255;
        titleTextColor->g = 255;
        titleTextColor->b = 255;
    }
    else
    {
        titleTextColor->r = 0;
        titleTextColor->g = 0;
        titleTextColor->b = 0;
    }
    return;
}

void displayTextWithShadow(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move, char *option_x, char *option_y)
{
    // option_x = "LEFT, "CENTER" or "RIGHT"
    // option_y = "TOP, "CENTER" or "BOTTOM"

    SDL_Color shadowColor = {0, 0, 0};
    displayText(renderer, font, text, shadowColor, x + move, y + move, option_x, option_y);
    displayText(renderer, font, text, titleTextColor, x, y, option_x, option_y);
}

void displayText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color titleTextColor, int32_t x, int32_t y, char *option_x, char *option_y)
{
    // option_x = "LEFT, "CENTER" or "RIGHT"
    // option_y = "TOP, "CENTER" or "BOTTOM"

    SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, text, titleTextColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度

    if (option_x == "CENTER")
    {
        x = x - text_width / 2;
    }
    else if (option_x == "RIGHT")
    {
        x = x - text_width;
    }

    if (option_y == "CENTER")
    {
        y = y - text_height / 2;
    }
    else if (option_y == "BOTTOM")
    {
        y = y - text_height;
    }

    SDL_Rect renderQuad = {x, y, text_width, text_height};    // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad); // 執行渲染操作

    SDL_FreeSurface(textSurface);    // 釋放表面資源
    SDL_DestroyTexture(textTexture); // 銷毀紋理資源

    return;
}

void backpackInit(backpackData *backpack_data, backpackItem **backpack_items)
{
    // 初始化背包
    backpack_data->backpack_item_count = 0;
    backpack_data->backpack_normal_block_width = 0.15;
    backpack_data->backpack_normal_block_height = 0.15;
    backpack_data->backpack_normal_block_gap = 0.05;
    backpack_data->backpack_01_x = 0.1;
    backpack_data->backpack_01_y = 0.1;
    backpack_data->backpack_02_x = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_02_y = 0.1;
    backpack_data->backpack_03_x = 0.1 + 0.15 + 0.05 + 0.15 + 0.05;
    backpack_data->backpack_03_y = 0.1;
    backpack_data->backpack_04_x = 0.1;
    backpack_data->backpack_04_y = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_05_x = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_05_y = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_06_x = 0.1 + 0.15 + 0.05 + 0.15 + 0.05;

    //item 初始化
    *backpack_items = (backpackItem *)malloc(sizeof(backpackItem) * 6);
    for (int i = 0; i < 6; i++)
    {
        (*backpack_items)[i].status = 0;
        strcpy((*backpack_items)[i].name, "");
        strcpy((*backpack_items)[i].description, "");
        strcpy((*backpack_items)[i].image_path, "");
    }

    return;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h, char *option_x, char *option_y)
{
    // option_x = "LEFT, "CENTER" or "RIGHT"
    // option_y = "TOP, "CENTER" or "BOTTOM"
    SDL_Rect dst;

    if (option_x == "CENTER")
    {
        x = x - w / 2;
    }
    else if (option_x == "RIGHT")
    {
        x = x - w;
    }

    if (option_y == "CENTER")
    {
        y = y - h / 2;
    }
    else if (option_y == "BOTTOM")
    {
        y = y - h;
    }

    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    return;
}