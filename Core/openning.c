#include "openning.h"

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

void openningDataInit(openningData *data)
{
    // 所有位置皆為比例
    data->title_text_color = 1;
    data->title_x = 0.5;
    data->title_y = 100.0 / 480.0;
    data->start_button_x = 0.5;
    data->start_button_y = 250.0 / 480.0;
    data->aboutus_button_x = data->start_button_x;
    data->aboutus_button_y = 330.0 / 480.0;
    data->llm_mode_button_x = data->start_button_x;
    data->llm_mode_button_y = 410.0 / 480.0;
    data->normal_button_width = 160.0 / 640.0;
    data->normal_button_height = 60.0 / 480.0;
    data->exit_button_x = 0.9675;
    data->exit_button_y = 0.0585;
    data->exit_button_width = 0.05;
    data->exit_button_height = 0.05;

    return;
}

int32_t openningMain(openningData *data, openningPath *path, SDL_Window *window)
{
    double option = 0; //-1 = Error, 0 = Quit, 1 = start, 2 = aboutus
    int32_t window_width, window_height;
    uint8_t title_text_color = data->title_text_color;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 開始頁面背景圖片
    SDL_Surface *loadedSurface = IMG_Load(path->backgroundPath);
    if (!loadedSurface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    // 字體
    TTF_Font *title_font = TTF_OpenFont(path->fontPath, 55);
    if (!title_font)
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 按鈕
    SDL_Surface *blackbuttonSurface = IMG_Load(path->blackButtonPath);
    if (!blackbuttonSurface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *blackbuttonTexture = SDL_CreateTextureFromSurface(renderer, blackbuttonSurface);
    SDL_SetTextureAlphaMod(blackbuttonTexture, 200); // 調整按鈕透明度
    SDL_FreeSurface(blackbuttonSurface);

    SDL_Surface *whitebuttonSurface = IMG_Load(path->whiteButtonPath);
    if (!whitebuttonSurface)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *whitebuttonTexture = SDL_CreateTextureFromSurface(renderer, whitebuttonSurface);
    SDL_SetTextureAlphaMod(whitebuttonTexture, 200); // 調整按鈕透明度
    SDL_FreeSurface(whitebuttonSurface);

    // 事件處理
    SDL_Event e;
    int32_t quit = 0;
    int32_t step = 0;
    int32_t mouse_x = 0, mouse_y = 0;

    while (!quit)
    {
        int32_t mouse_press = 0;
        while (SDL_PollEvent(&e) != 0 && !quit)
        {
            switch (e.type)
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
            SDL_GetMouseState(&mouse_x, &mouse_y); // 處理鼠標位置
        }

        // 渲染背景
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL); // 渲染背景

        // 取得視窗大小
        SDL_GetWindowSize(window, &window_width, &window_height);
        // printf("window_width = %d, window_height = %d\n", window_width, window_height);

        // 改變參數
        double title_x = data->title_x * (double)window_width;
        double title_y = data->title_y * (double)window_height;
        double start_button_x = data->start_button_x * (double)window_width;
        double start_button_y = data->start_button_y * (double)window_height;
        double aboutus_button_x = data->aboutus_button_x * (double)window_width;
        double aboutus_button_y = data->aboutus_button_y * (double)window_height;
        double llm_mode_button_x = data->llm_mode_button_x * (double)window_width;
        double llm_mode_button_y = data->llm_mode_button_y * (double)window_height;
        double normal_button_width = data->normal_button_width * (double)window_width;
        double normal_button_height = data->normal_button_height * (double)window_height;
        double exit_button_x = data->exit_button_x * (double)window_width;
        double exit_button_y = data->exit_button_y * (double)window_height;
        double exit_button_width = data->exit_button_width * (double)window_width;
        double exit_button_height = exit_button_width;

        // 渲染標題字
        SDL_Color titleTextColor;
        chooseTextColor(title_text_color, &titleTextColor);
        title_font = TTF_OpenFont(path->fontPath, 35 * window_width * window_height / (2.2 * 640.0 * 480.0));
        displayTextWithShadow(renderer, title_font, data->title, titleTextColor, title_x, title_y, 5, "CENTER", "TOP");

        // SDL_RenderPresent(renderer);
        // renderTexture(dialog_box_texture, renderer, dialogBox_start_x, dialogBox_start_y, 640, 130);

        // start button
        SDL_Color startTextColor;
        title_font = TTF_OpenFont(path->fontPath, 30 * window_width * window_height / (3 * 640.0 * 480.0));
        if (judgeButtonPressed(mouse_x, mouse_y, start_button_x, start_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER"))
        {
            chooseTextColor(0, &startTextColor);
            renderTexture(whitebuttonTexture, renderer, start_button_x, start_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "Start", startTextColor, start_button_x, start_button_y, "CENTER", "CENTER");

            if (mouse_press == 1)
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
            renderTexture(blackbuttonTexture, renderer, start_button_x, start_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "Start", startTextColor, start_button_x, start_button_y, "CENTER", "CENTER");
        }

        // aboutus button
        SDL_Color aboutusTextColor;
        title_font = TTF_OpenFont(path->fontPath, 30 * window_width * window_height / (3 * 640.0 * 480.0));
        if (judgeButtonPressed(mouse_x, mouse_y, aboutus_button_x, aboutus_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER"))
        {
            chooseTextColor(0, &aboutusTextColor);
            renderTexture(whitebuttonTexture, renderer, aboutus_button_x, aboutus_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "Aboutus", aboutusTextColor, aboutus_button_x, aboutus_button_y, "CENTER", "CENTER");
            // printf("mouse on aboutus button\n");
            if (mouse_press == 1)
            {
                printf("aboutus button is pressed\n");
                quit = 1;
                option = 2;
            }
        }
        else
        {
            chooseTextColor(1, &aboutusTextColor);
            renderTexture(blackbuttonTexture, renderer, aboutus_button_x, aboutus_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "Aboutus", aboutusTextColor, aboutus_button_x, aboutus_button_y, "CENTER", "CENTER");
        }

        // llm mode button
        SDL_Color llmTextColor;
        title_font = TTF_OpenFont(path->fontPath, 30 * window_width * window_height / (3 * 640.0 * 480.0));
        if (judgeButtonPressed(mouse_x, mouse_y, llm_mode_button_x, llm_mode_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER"))
        {
            chooseTextColor(0, &llmTextColor);
            renderTexture(whitebuttonTexture, renderer, llm_mode_button_x, llm_mode_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "LLM Mode", llmTextColor, llm_mode_button_x, llm_mode_button_y, "CENTER", "CENTER");
            // printf("mouse on llm mode button\n");
            if (mouse_press == 1)
            {
                printf("llm mode button is pressed\n");
                quit = 1;
                option = 3;
            }
        }
        else
        {
            chooseTextColor(1, &llmTextColor);
            renderTexture(blackbuttonTexture, renderer, llm_mode_button_x, llm_mode_button_y, normal_button_width, normal_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "LLM Mode", llmTextColor, llm_mode_button_x, llm_mode_button_y, "CENTER", "CENTER");
        }

        // exit button
        SDL_Color exitTextColor;
        title_font = TTF_OpenFont(path->fontPath, 35 * window_width * window_height / (3 * 640.0 * 480.0));
        if (judgeButtonPressed(mouse_x, mouse_y, exit_button_x, exit_button_y, exit_button_width, exit_button_height, "CENTER", "CENTER"))
        {
            chooseTextColor(0, &exitTextColor);
            renderTexture(whitebuttonTexture, renderer, exit_button_x, exit_button_y, exit_button_width, exit_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "×", exitTextColor, exit_button_x, exit_button_y, "CENTER", "CENTER");
            // printf("mouse on exit button\n");
            if (mouse_press == 1)
            {
                printf("exit button is pressed\n");
                quit = 1;
                option = 0;
            }
        }
        else
        {
            chooseTextColor(1, &exitTextColor);
            renderTexture(blackbuttonTexture, renderer, exit_button_x, exit_button_y, exit_button_width, exit_button_height, "CENTER", "CENTER");
            displayText(renderer, title_font, "×", exitTextColor, exit_button_x, exit_button_y, "CENTER", "CENTER");
        }

        // 更新畫面
        SDL_RenderPresent(renderer);
        TTF_CloseFont(title_font);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    if (option==1)
    {
        SDL_RenderClear(renderer);
    }
    return option;
}

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

    return;
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

uint8_t judgeButtonPressed(int32_t x, int32_t y, int32_t button_x, int32_t button_y, int32_t normal_button_width, int32_t normal_button_height, char *option_x, char *option_y)
{
    // option_x = "LEFT, "CENTER" or "RIGHT"
    // option_y = "TOP, "CENTER" or "BOTTOM"

    if (option_x == "CENTER")
    {
        button_x = button_x - normal_button_width / 2;
    }
    else if (option_x == "RIGHT")
    {
        button_x = button_x - normal_button_width;
    }

    if (option_y == "CENTER")
    {
        button_y = button_y - normal_button_height / 2;
    }
    else if (option_y == "BOTTOM")
    {
        button_y = button_y - normal_button_height;
    }

    if (x >= button_x && x <= button_x + normal_button_width && y >= button_y && y <= button_y + normal_button_height)
    {
        return 1;
    }
    return 0;
}