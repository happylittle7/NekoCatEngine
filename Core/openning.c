#include "openning.h"

void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, tex, NULL, &dst);
}

void openningDataInit(openningData* data){
    data->title_text_color = 1;
    data->title_x = 140.0/640.0;
    data->title_y = 100.0/480.0;
    data->window_width = 640.0;
    data->window_height = 480.0;
    data->start_button_x = 240.0/640.0;
    data->start_button_y = 250.0/480.0;
    data->aboutus_button_x = data->start_button_x;
    data->aboutus_button_y = data->start_button_y + 80.0/480.0;
    data->button_width = 160.0/640.0;
    data->button_height = 60.0/480.0;
}

int32_t openningMain(openningData *data, openningPath *path, SDL_Window* window) 
{
    uint8_t title_text_color = data->title_text_color;
    double title_x = data->title_x;
    double title_y = data->title_y;
    int32_t window_width = data->window_width;
    int32_t window_height = data->window_height;
    double start_button_x = data->start_button_x;
    double start_button_y = data->start_button_y;
    double aboutus_button_x = data->aboutus_button_x;
    double aboutus_button_y = data->aboutus_button_y;
    double button_width = data->button_width;
    double button_height = data->button_height;
    double option = 0; //-1 = Error, 0 = Quit, 1 = start, 2 = aboutus

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) 
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //開始頁面背景圖片
    SDL_Surface* loadedSurface = IMG_Load(path->backgroundPath);
    if (!loadedSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    //字體
    TTF_Font* title_font = TTF_OpenFont(path->fontPath, 55 * window_width / 640.0);
    if (!title_font) 
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //按鈕
    SDL_Surface* blackbuttonSurface = IMG_Load(path->blackButtonPath);
    if (!blackbuttonSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture* blackbuttonTexture = SDL_CreateTextureFromSurface(renderer, blackbuttonSurface);
    SDL_SetTextureAlphaMod(blackbuttonTexture, 200); //調整按鈕透明度
    SDL_FreeSurface(blackbuttonSurface);

    SDL_Surface* whitebuttonSurface = IMG_Load(path->whiteButtonPath);
    if (!whitebuttonSurface) 
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture* whitebuttonTexture = SDL_CreateTextureFromSurface(renderer, whitebuttonSurface);
    SDL_SetTextureAlphaMod(whitebuttonTexture, 200); //調整按鈕透明度
    SDL_FreeSurface(whitebuttonSurface);

    //事件處理
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

        SDL_GetWindowSize(window, &window_width, &window_height);
        title_x = 140.0/640.0 * (double)window_width;
        title_y = 100.0/480.0 * (double)window_height;
        start_button_x = 240.0/640.0 * (double)window_width;
        start_button_y = 250.0/480.0 * (double)window_height;
        aboutus_button_x = start_button_x;
        aboutus_button_y = start_button_y + 80.0/480.0 * (double)window_height;
        button_width = 160.0/640.0 * (double)window_width;
        button_height = 60.0/480.0 * (double)window_height;

        //渲染背景
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);  // 渲染背景

        //渲染標題字
        SDL_Color titleTextColor;
        chooseTextColor(title_text_color, &titleTextColor);
        title_font = TTF_OpenFont("./Assets/font/Cubic_11_1.100_R.ttf", 55 * window_width * window_height / (2.2*640.0 * 480.0));
        displayTextWithShadow(renderer, title_font, data->title, titleTextColor, title_x, title_y, 5);
        // SDL_RenderPresent(renderer);
        // renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);

        //start button
        SDL_Color startTextColor;
        title_font = TTF_OpenFont("./Assets/font/Cubic_11_1.100_R.ttf", 30 * window_width * window_height / (3*640.0 * 480.0));
        if(mouse_x >= start_button_x && mouse_x <= start_button_x + button_width && mouse_y >= start_button_y && mouse_y <= start_button_y + button_height)
        {
            chooseTextColor(0, &startTextColor);
            renderTexture(whitebuttonTexture, renderer, start_button_x, start_button_y, button_width, button_height);
            displayText(renderer, title_font, "Start", startTextColor, start_button_x + 57 * window_width / 640.0, start_button_y + 10 * window_height / 480.0);

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
            displayText(renderer, title_font, "Start", startTextColor, start_button_x + 57 * window_width / 640.0, start_button_y + 10 * window_height / 480.0);
        }

        //aboutus button
        SDL_Color aboutusTextColor;
        title_font = TTF_OpenFont("./Assets/font/Cubic_11_1.100_R.ttf", 30 * window_width * window_height / (3*640.0 * 480.0));
        if(mouse_x >= aboutus_button_x && mouse_x <= aboutus_button_x + button_width && mouse_y >= aboutus_button_y && mouse_y <= aboutus_button_y + button_height)
        {
            chooseTextColor(0, &aboutusTextColor);
            renderTexture(whitebuttonTexture, renderer, aboutus_button_x, aboutus_button_y, button_width, button_height);
            displayText(renderer, title_font, "Aboutus", aboutusTextColor, aboutus_button_x + 37 * window_width / 640.0, aboutus_button_y + 10 * window_height / 480.0);
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
            displayText(renderer, title_font, "Aboutus", aboutusTextColor, aboutus_button_x + 37 * window_width / 640.0, aboutus_button_y + 10 * window_height / 480.0);
        }

        //更新畫面
        SDL_RenderPresent(renderer);
        TTF_CloseFont(title_font);
    }
 
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    return option;
}

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

void displayTextWithShadow(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y, int32_t move)
{
    SDL_Color shadowColor = {0, 0, 0};
    displayText(renderer, font, text, shadowColor, x+move, y+move);
    displayText(renderer, font, text, titleTextColor, x, y);
}

void displayText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color titleTextColor, int32_t x, int32_t y) 
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, titleTextColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;  // 文本寬度
    int text_height = textSurface->h; // 文本高度

    SDL_Rect renderQuad = { x, y, text_width, text_height };  // 定義渲染區域
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作

    SDL_FreeSurface(textSurface);  // 釋放表面資源
    SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
}