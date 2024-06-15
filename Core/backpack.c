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

void myDisplayTextWithShadow(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move, char *option_x, char *option_y)
{
    // option_x = "LEFT, "CENTER" or "RIGHT"
    // option_y = "TOP, "CENTER" or "BOTTOM"

    SDL_Color shadowColor = {0, 0, 0};
    myDisplayText(renderer, font, text, shadowColor, x + move, y + move, option_x, option_y);
    myDisplayText(renderer, font, text, titleTextColor, x, y, option_x, option_y);
}

void myDisplayText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color titleTextColor, int32_t x, int32_t y, char *option_x, char *option_y)
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

void backpackDataInit(backpackData *backpack_data)
{
    // 初始化背包
    backpack_data->backpack_item_count = 0;
    backpack_data->backpack_normal_block_width = 0.15;
    backpack_data->backpack_normal_block_height = 0.15;
    backpack_data->backpack_normal_block_gap = 0.05;
    backpack_data->backpack_xy[0][0] = 0.1;
    backpack_data->backpack_xy[0][1] = 0.1;
    backpack_data->backpack_xy[1][0] = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_xy[1][1] = 0.1;
    backpack_data->backpack_xy[2][0] = 0.1 + 0.15 + 0.05 + 0.15 + 0.05;
    backpack_data->backpack_xy[2][1] = 0.1;
    backpack_data->backpack_xy[3][0] = 0.1;
    backpack_data->backpack_xy[3][1] = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_xy[4][0] = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_xy[4][1] = 0.1 + 0.15 + 0.05;
    backpack_data->backpack_xy[5][0] = 0.1 + 0.15 + 0.05 + 0.15 + 0.05;
    backpack_data->backpack_xy[5][1] = 0.1 + 0.15 + 0.05;

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

int32_t backpackMain(backpackData *data, backpackItem *items, backpackPath *path, RenderResources* resources, SDL_Window *window){

    double option = 0; //-1 = Error, 0 = Quit the game, 1 = Leave the backpack 
    int32_t window_width, window_height;
    uint8_t title_text_color = 0;

    SDL_Renderer *backpack_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!backpack_renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // background
    SDL_Surface *background_surface = IMG_Load(path->background_path);
    if (!background_surface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(backpack_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    resources->background_texture = SDL_CreateTextureFromSurface(backpack_renderer, background_surface);
    SDL_FreeSurface(background_surface);

    // font
    TTF_Font *font = TTF_OpenFont(path->font_path, 24);
    if (!font)
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(backpack_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // backpack_block
    SDL_Surface *backpack_block_surface = IMG_Load(path->black_block_path);
    if (!backpack_block_surface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(backpack_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    resources->backpack_block_texture = SDL_CreateTextureFromSurface(backpack_renderer, backpack_block_surface);
    SDL_SetTextureAlphaMod(resources->backpack_block_texture, 200); // 調整透明度
    SDL_FreeSurface(backpack_block_surface);

    // backpack_edge_block
    SDL_Surface *backpack_edge_block_surface = IMG_Load(path->white_edge_black_block_path);
    if (!backpack_edge_block_surface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(backpack_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    resources->white_edge_black_block_texture = SDL_CreateTextureFromSurface(backpack_renderer, backpack_edge_block_surface);
    SDL_FreeSurface(backpack_edge_block_surface);
    SDL_SetTextureAlphaMod(resources->white_edge_black_block_texture, 200); // 調整透明度

    // items

    for(int32_t i = 0; i < 6; i++){
        if(items[0].status == 0){
            resources->item_texture[i] = NULL;
            continue;
        }
        SDL_Surface *item_surface = IMG_Load(items[i].image_path);
        if (!item_surface)
        {
            printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
            SDL_DestroyRenderer(backpack_renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }
        resources->item_texture[i] = SDL_CreateTextureFromSurface(backpack_renderer, item_surface);
        SDL_FreeSurface(item_surface);
    }

    // 事件處理
    SDL_Event e;
    int32_t quit = 0;
    int32_t choose = 0;
    int32_t mouse_x = 0, mouse_y = 0;

    while(!quit){
        int32_t pressed = 0;
        while(SDL_PollEvent(&e) != 0 && !quit){
            switch(e.type){
                case SDL_QUIT:
                    quit = 1;
                    option = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(e.button.button == SDL_BUTTON_LEFT){
                        pressed = 1;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_e:
                            printf("E key pressed\n");
                            quit = 1;
                            option = 7;
                            break;
                        default:
                            break;
                    }
                break;
            }
            // 獲取滑鼠位置
            SDL_GetMouseState(&mouse_x, &mouse_y);
        }

        // 渲染背景
        SDL_RenderClear(backpack_renderer);
        SDL_RenderCopy(backpack_renderer, resources->background_texture, NULL, NULL);
        
        // 取得視窗大小
        SDL_GetWindowSize(window, &window_width, &window_height);

        // 改變參數
        double block_width = data->backpack_normal_block_width * window_width;
        double block_height = data->backpack_normal_block_height * window_height;
        double block_gap = data->backpack_normal_block_gap * window_width;
        double x[6],y[6];

        for(int32_t i = 0;i < 6; i++){
            x[i] = data->backpack_xy[i][0] * window_width;
            y[i] = data->backpack_xy[i][1] * window_height;
        }

        // 渲染背包物品
        for(int32_t i = 0; i < 6; i++){
            if(items[i].status == 2){
                renderTexture(resources->white_edge_black_block_texture, backpack_renderer, x[i], y[i], block_width, block_height, "LEFT", "TOP");
            }else{
                renderTexture(resources->backpack_block_texture, backpack_renderer, x[i], y[i], block_width, block_height, "LEFT", "TOP");
            }
            if(items[i].status == 0)continue;
            renderTexture(resources->item_texture[i], backpack_renderer, x[i] + block_width / 2.0, y[i] + block_height / 2.0, block_width, block_height, "CENTER", "CENTER");
        }

        // 更新畫面
        SDL_RenderPresent(backpack_renderer);
    }
    
    return option;
}

int32_t backpackItemInit(backpackItem **items, int32_t **status, char ***name, char ***description, char ***image_path){
    
    if(items == NULL || status == NULL || name == NULL || description == NULL || image_path == NULL){
        return -1;
    }

    for(int32_t i = 0; i < 6; i++){
        items[i]->status = *(status[i]);
        strcpy(items[i]->name, *(name[i]));
        strcpy(items[i]->description, *(description[i]));
        char *temp = "./item.nekocat/";
        strcat(temp, *(image_path[i]));
        strcpy(items[i]->image_path, temp);
        temp = NULL;
    }
    return 0;
}