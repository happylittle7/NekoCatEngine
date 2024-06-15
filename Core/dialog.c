#include "dialog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
/*
void initButton(SDL_Renderer* renderer, TTF_Font* font, Button* button, SDL_Rect rect, SDL_Color color, const char* text) {
    button->rect = rect;
    button->color = color;
    button->text = text;
    button->font = font;

    // 创建纹理
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, (SDL_Color){255, 255, 255, 255});
    button->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
*/
void MakeOption(RenderResources* resources, SDL_Renderer* renderer, const char* optionContent_id, TTF_Font* font, int index, int totalOptions) {
    // 初始化资源中的按钮数组
    //printf("A\n");
    // 确保 resources->now_option_button 已经分配内存
    if (resources->now_option_button == NULL) {
        resources->now_option_button = (Button**)calloc(totalOptions, sizeof(Button*));
    }

    // 分配每个按钮的内存
    if (resources->now_option_button[index] == NULL) {
        resources->now_option_button[index] = (Button*)malloc(sizeof(Button));
    }

    // 定义按钮的宽度和高度
    int buttonWidth = 400;
    int buttonHeight = 50;

    // 计算每个按钮的位置
    int buttonX = 100; // 按钮的 X 位置
    int buttonY = 100 + (index * (buttonHeight + 20)); // 按钮的 Y 位置
    //printf("B\n");
    // 初始化按钮的矩形区域
    SDL_Rect buttonRect = {buttonX*width_ratio, buttonY*height_ratio, buttonWidth*width_ratio, buttonHeight*height_ratio};

    // 创建按钮的背景纹理
    SDL_Texture* buttonBgTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, buttonWidth, buttonHeight);
    SDL_SetRenderTarget(renderer, buttonBgTexture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色背景
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    //printf("C\n");
    // 创建按钮的文字纹理
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, optionContent_id, (SDL_Color){0, 0, 0, 255}); // 黑色文字
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    //printf("D\n");
    // 获取文字的宽度和高度
    int textWidth = textSurface->w*width_ratio;
    int textHeight = textSurface->h*height_ratio;
    SDL_FreeSurface(textSurface);
    //printf("E\n");
    // 计算文字在按钮中的位置
    SDL_Rect textRect = {buttonX + (buttonWidth - textWidth) / 2, buttonY + (buttonHeight - textHeight) / 2, textWidth, textHeight};
    //printf("F\n");
    // 设置按钮对象的属性
    resources->now_option_button[index]->text_texture = textTexture;
    resources->now_option_button[index]->text_rect = textRect;
    resources->now_option_button[index]->IMG_texture = buttonBgTexture;
    resources->now_option_button[index]->IMG_rect = buttonRect;
    //printf("E\n");
    
}
void change_ratio(int32_t a, int32_t b, int32_t c, int32_t d)
{
    width_ratio = (float)a/b;
    height_ratio = (float)c/d;
    //printf("%f %f\n",width_ratio,height_ratio);
}
int32_t checkLeaveButton(SDL_Event* e, Button* leave_button) 
{
    if (e->type == SDL_MOUSEBUTTONDOWN) 
    {
        int32_t x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= leave_button->IMG_rect.x &&
            x <= leave_button->IMG_rect.x + leave_button->IMG_rect.w &&
            y >= leave_button->IMG_rect.y &&
            y <= leave_button->IMG_rect.y + leave_button->IMG_rect.h) {
            return 1; // Leave button clicked
        }
    }
    return 0; // Leave button not clicked
}
/*
int32_t handleButtonEvents(SDL_Event* e, RenderResources* resources, int32_t num_option_buttons) 
{
    if (e->type == SDL_MOUSEBUTTONDOWN) 
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < num_option_buttons; ++i) 
        {
            if (resources->now_option_button[i] &&
                x >= resources->now_option_button[i]->rect.x &&
                x <= resources->now_option_button[i]->rect.x + resources->now_option_button[i]->rect.w &&
                y >= resources->now_option_button[i]->rect.y &&
                y <= resources->now_option_button[i]->rect.y + resources->now_option_button[i]->rect.h) 
            {
                return i; // Option button i clicked
            }
        }
    }
    return -1; // No button clicked
}
*/

int32_t handleButtonEvents(SDL_Event* e, RenderResources* resources) 
{
    if (e->type == SDL_MOUSEBUTTONDOWN) 
    {
        int mouseX = e->button.x;
        int mouseY = e->button.y;

        // Check leave button
        if (resources->leave_button && mouseX >= resources->leave_button->IMG_rect.x && mouseX <= resources->leave_button->IMG_rect.x + resources->leave_button->IMG_rect.w &&
            mouseY >= resources->leave_button->IMG_rect.y && mouseY <= resources->leave_button->IMG_rect.y + resources->leave_button->IMG_rect.h) 
        {
            return -1; // Special value for the leave button
        }

        // Check option buttons
        for (int i = 0; i < 3; ++i) {
            if (resources->now_option_button[i] && mouseX >= resources->now_option_button[i]->IMG_rect.x && mouseX <= resources->now_option_button[i]->IMG_rect.x + resources->now_option_button[i]->IMG_rect.w &&
                mouseY >= resources->now_option_button[i]->IMG_rect.y && mouseY <= resources->now_option_button[i]->IMG_rect.y + resources->now_option_button[i]->IMG_rect.h) {
                return i; // Return the index of the button that was clicked
            }
        }
    }

    return -2; // Return -2 if no button was clicked
}
void initRenderResources(RenderResources* resources) {
    resources->background_texture = NULL;
    resources->dialog_box_texture = NULL;
    resources->text_texture = NULL;
    resources->character_IMG_texture = (SDL_Texture**)malloc(3 * sizeof(SDL_Texture*));
    resources->character_IMG_renderQuads = (SDL_Rect**)calloc(3, sizeof(SDL_Rect*));
    for (int i = 0; i < 3; ++i) {
        resources->character_IMG_texture[i] = NULL;
        resources->character_IMG_renderQuads[i] = (SDL_Rect*)malloc(sizeof(SDL_Rect));  // 分配内存
    }
    resources->expression_texture = NULL;  // 新增
    resources->leave_button = NULL;  // 新增
    //resources->now_option_button = NULL;  // 初始化为NULL
    resources->now_option_button = (Button**)calloc(3, sizeof(Button*));  // 分配三个空间并初始化为NULL
}
void freeRenderResources(RenderResources* resources) {
    if (!resources) return;
    
    if (resources->background_texture) {
        SDL_DestroyTexture(resources->background_texture);
        resources->background_texture = NULL;
    }
    
    if (resources->dialog_box_texture) {
        SDL_DestroyTexture(resources->dialog_box_texture);
        resources->dialog_box_texture = NULL;
    }
    
    if (resources->text_texture) {
        SDL_DestroyTexture(resources->text_texture);
        resources->text_texture = NULL;
    }
    
    if (resources->character_IMG_texture) {
        for (int i = 0; i < 3; i++) {
            if (resources->character_IMG_texture[i]) {
                SDL_DestroyTexture(resources->character_IMG_texture[i]);
            }
        }
        free(resources->character_IMG_texture);
        resources->character_IMG_texture = NULL;
    }
    
    if (resources->character_IMG_renderQuads) {
        for (int i = 0; i < 3; i++) {
            if (resources->character_IMG_renderQuads[i]) {
                free(resources->character_IMG_renderQuads[i]);
            }
        }
        free(resources->character_IMG_renderQuads);
        resources->character_IMG_renderQuads = NULL;
    }
    
    if (resources->expression_texture) {
        SDL_DestroyTexture(resources->expression_texture);
        resources->expression_texture = NULL;
    }
    
    if (resources->leave_button) {
        if (resources->leave_button->text_texture) {
            SDL_DestroyTexture(resources->leave_button->text_texture);
        }
        if (resources->leave_button->IMG_texture) {
            SDL_DestroyTexture(resources->leave_button->IMG_texture);
        }
        free(resources->leave_button);
        resources->leave_button = NULL;
    }
    
    if (resources->now_option_button) {
        for (int i = 0; i < 3; ++i) {  // 假设有3个选项按钮
            if (resources->now_option_button[i]) {
                if (resources->now_option_button[i]->text_texture) {
                    SDL_DestroyTexture(resources->now_option_button[i]->text_texture);
                }
                if (resources->now_option_button[i]->IMG_texture) {
                    SDL_DestroyTexture(resources->now_option_button[i]->IMG_texture);
                }
                free(resources->now_option_button[i]);
            }
        }
        free(resources->now_option_button);
        resources->now_option_button = NULL;
    }
}
void renderButton(SDL_Renderer* renderer, Button* button) {
    // 渲染按鈕圖像
    if (button->IMG_texture) {
        SDL_RenderCopy(renderer, button->IMG_texture, NULL, &button->IMG_rect);
    }

    // 渲染按鈕文字
    if (button->text_texture) {
        SDL_RenderCopy(renderer, button->text_texture, NULL, &button->text_rect);
    }
}
void my_RenderPresent(SDL_Renderer* renderer, RenderResources* resources, int32_t now_state) 
{
    // 渲染背景
    //printf("A\n");
    //printf("%d\n",now_state);
    if (resources->background_texture) 
    {
        //printf("background_texture is OK\n");
        if (now_state==2 || now_state==5)
            SDL_RenderCopy(renderer, resources->background_texture, NULL, NULL);
    }
    
    // 渲染文字框
    if (resources->dialog_box_texture) 
    {
        //printf("dialog_box_texture is OK\n");
        if (now_state == 2 || now_state==5)
            SDL_RenderCopy(renderer, resources->dialog_box_texture, NULL, &resources->dialog_box_renderQuad);
    }
    
    // 渲染角色立绘
    if (resources->character_IMG_texture) 
    {
        //printf("character_IMG_texture is OK\n");
        if (now_state == 2)
            for (int32_t i=0 ; i<3 ; i++)
            {
                if (resources->character_IMG_texture[i]!=NULL)
                {
                    //printf("here\n");
                    SDL_RenderCopy(renderer, resources->character_IMG_texture[i], NULL, (resources->character_IMG_renderQuads[i])); // 此處尚未完成
                }
            }
        
    }
    //printf("B\n");
    // 渲染表情
    if (resources->expression_texture) 
    {
        //printf("expression_texture is OK\n");
        if (now_state == 2)
            SDL_RenderCopy(renderer, resources->expression_texture, NULL, &resources->expression_renderQuad);
    }
    // 渲染文字
    if (resources->text_texture) 
    {
        //printf("text_texture is OK\n");
        SDL_RenderCopy(renderer, resources->text_texture, NULL, &resources->text_renderQuad);
    }

    // Option模式
    printf("now state=%d\n",now_state);
    for (int32_t i = 0; i < 3; i++) 
    {
        if (now_state == 3)
        {
            printf("here_now_state = %d\n",now_state);
            if (resources->now_option_button[i]!=NULL) 
            {
                renderButton(renderer, resources->now_option_button[i]);
            }
        }
            
    }
    if (resources->leave_button->text_texture) 
    {
        renderButton(renderer, resources->leave_button);
    }
    
    // 更新屏幕
    SDL_RenderPresent(renderer);
}
/*
SDL_Texture* copyTexture(SDL_Renderer* renderer, SDL_Texture* srcTexture) //紋理深拷貝
{
    int width, height;
    SDL_QueryTexture(srcTexture, NULL, NULL, &width, &height);

    SDL_Texture* dstTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

    SDL_SetRenderTarget(renderer, dstTexture);
    SDL_RenderCopy(renderer, srcTexture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    return dstTexture;
}
*/
// 顯示文本的函數，負責將指定文本渲染到指定位置，暫時不考慮文字太常超過對話框的h時的下拉式處理
void dialogText(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w) 
{
    char *print_char = (*text);
    SDL_Surface* textSurface;
    //SDL_Texture* textTexture;
    int32_t now_x = *x;
    int32_t now_y = *y;
    //y+=24;
    char my_text[2] = {0};
    my_text[0] = *print_char;
    print_char++;
    *text = print_char;
    
    textSurface = TTF_RenderUTF8_Blended(font, my_text, textColor);
    resources->text_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureAlphaMod(resources->text_texture, 255);
    int32_t text_width = textSurface->w;  // 文本寬度
    int32_t text_height = textSurface->h; // 文本高度
    //resources->text_texture = textTexture;
    SDL_Rect renderQuad = { now_x*width_ratio, now_y*height_ratio, text_width*width_ratio, text_height*height_ratio };  // 定義渲染區域
    resources->text_renderQuad = renderQuad;
    //SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
    //SDL_RenderPresent(renderer);
    SDL_Delay(50);
    now_x += text_width;
    if (now_x >= max_w)
    {
        now_y += text_height;
        now_x = *x;
    }
    *x = now_x;
    *y = now_y;
    //SDL_RenderClear(renderer);
    //SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
    SDL_FreeSurface(textSurface);  // 釋放表面資源
}

void dialogText_2(SDL_Renderer* renderer,RenderResources* resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w) //滑鼠點擊後全顯示
{
    SDL_Surface* textSurface;
    //SDL_Texture* textTexture;
    char *print_char = (*text);
    textSurface = TTF_RenderUTF8_Blended(font, print_char, textColor);
    resources->text_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureAlphaMod(resources->text_texture, 255);
    int32_t text_width = textSurface->w;  // 文本寬度
    int32_t text_height = textSurface->h; // 文本高度
    int32_t now_x = *x;
    int32_t now_y = *y;
    if (now_x >= max_w)
    {
        now_y += text_height;
        now_x = *x;
    }
    SDL_Rect renderQuad = { now_x*width_ratio, now_y*height_ratio, text_width*width_ratio, text_height*height_ratio };  // 定義渲染區域
    //resources->text_texture = textTexture;
    resources->text_renderQuad = renderQuad;
    //SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
    *print_char = 0;
    *text = print_char;
    //SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
    SDL_FreeSurface(textSurface);  // 釋放表面資源
}

///圖片縮放後再使其渲染到render
void SetDialogBox(SDL_Renderer* renderer , RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha)
{
    SDL_SetTextureAlphaMod(resources -> dialog_box_texture, alpha);
    //resources -> dialog_box_texture = tex;
    SDL_Rect* dst = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    dst->x = (int32_t)((float)x * width_ratio);
    dst->y = (int32_t)((float)y * height_ratio);
    dst->w = (int32_t)((float)w * width_ratio);
    dst->h = (int32_t)((float)h * height_ratio);
    //printf("%d %d %d %d\n",dst->x,dst->y,dst->w,dst->h);
    resources -> dialog_box_renderQuad = *dst;
    free(dst);
}
void SetExpression(SDL_Renderer* renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t alpha)
{
    SDL_SetTextureAlphaMod(resources -> expression_texture, alpha);
    //resources -> expression_texture = tex;
    SDL_Rect* dst = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    dst->x = (int32_t)((float)x * width_ratio);
    dst->y = (int32_t)((float)y * height_ratio);
    dst->w = (int32_t)((float)w * width_ratio);
    dst->h = (int32_t)((float)h * height_ratio);
    //printf("%d %d %d %d\n",dst->x,dst->y,dst->w,dst->h);
    resources -> expression_renderQuad = *dst;
    free(dst);
}
void SetCharacterImg(SDL_Renderer* renderer, RenderResources *resources, int32_t x, int32_t y, int32_t w, int32_t h, int32_t idx, uint8_t alpha) {
    SDL_SetTextureAlphaMod(resources->character_IMG_texture[idx], alpha);
    resources->character_IMG_renderQuads[idx]->x = (int32_t)((float)x * width_ratio);
    resources->character_IMG_renderQuads[idx]->y = (int32_t)((float)y * height_ratio);
    resources->character_IMG_renderQuads[idx]->w = (int32_t)((float)w * width_ratio);
    resources->character_IMG_renderQuads[idx]->h = (int32_t)((float)h * height_ratio);
    //printf("%d %d %d %d\n",dst->x,dst->y,dst->w,dst->h);
}
void displayIMG(SDL_Renderer* renderer, RenderResources *resources, toml_table_t *Character, char* directory, const char* Character_id, const char* mood, int32_t idx, int32_t number) {
    char* Character_id_cpy = (char*)malloc(1024 * sizeof(char));
    strcpy(Character_id_cpy, Character_id);
    toml_table_t *now_character = toml_table_in(Character, Character_id_cpy);
    if (!now_character) {
        fprintf(stderr, "Error: Character not found for command '%s'\n", Character_id_cpy);
        free(Character_id_cpy);
        return;
    }
    toml_array_t *sprites_list = toml_array_in(now_character, "sprites_list");
    if (!sprites_list) {
        fprintf(stderr, "Error: Sprites list not found in character for command '%s'\n", Character_id_cpy);
        free(Character_id_cpy);
        return;
    }
    toml_array_t *sprites_png_list = toml_array_in(now_character, "sprites_png_list");
    if (!sprites_png_list) {
        fprintf(stderr, "Error: Sprites PNG list not found in character for command '%s'\n", Character_id_cpy);
        free(Character_id_cpy);
        return;
    }
    int32_t emoSize = toml_array_nelem(sprites_list);
    for (int32_t i = 0; i < emoSize; i++) {
        const char* now_emo = toml_string_at(sprites_list, i).u.s;
        if (strcmp(now_emo, mood) == 0) {
            const char* now_emo_path = toml_string_at(sprites_png_list, i).u.s;
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "./Assets/%s/character/%s", directory, now_emo_path);
            ///now_emo_path要加上./character.nekocat/image/
            SDL_Surface* character_IMG = IMG_Load(full_path);
            if (!character_IMG) 
            {
                fprintf(stderr, "Error: Unable to load image '%s': %s\n", full_path, IMG_GetError());
                continue;
            }
            resources->character_IMG_texture[idx] = SDL_CreateTextureFromSurface(renderer, character_IMG);
            SDL_FreeSurface(character_IMG);
            if (!resources->character_IMG_texture[idx]) {
                fprintf(stderr, "Error: Unable to create texture from surface: %s\n", SDL_GetError());
                continue;
            }
            if (number == 1)
            {
                int32_t character_IMG_start_x = 300;
                int32_t character_IMG_start_y = 50;
                SetCharacterImg(renderer, resources, character_IMG_start_x + 200 * idx, character_IMG_start_y, 200, 300, idx, 255);
            }
            else if (number==2)
            {
                int32_t character_IMG_start_x = 100;
                int32_t character_IMG_start_y = 50;
                SetCharacterImg(renderer, resources, character_IMG_start_x + 200 * idx, character_IMG_start_y, 200, 300, idx, 255);
            }
            else
            {
                int32_t character_IMG_start_x = 50;
                int32_t character_IMG_start_y = 50;
                SetCharacterImg(renderer, resources, character_IMG_start_x + 200 * idx, character_IMG_start_y, 200, 300, idx, 255);
            }
            break;
        }
    }
    free(Character_id_cpy);
}




///清除顯示完的文字
void clearAndRender(RenderResources *resources, SDL_Texture *backgroundTexture ) 
{
    resources->background_texture = backgroundTexture;
    resources->text_texture = NULL;
    //SDL_Delay(2000);

}
///顯示Expression
void DisplayTheExpression(SDL_Renderer* renderer, RenderResources *resources, toml_table_t *Character, char* directory, const char* command, const char* mood) ///此段函數要在左上角印出腳色圖片
{
    char* command_cpy = calloc(1024,sizeof(char));
    strcpy(command_cpy,command);
    command_cpy++;
    command_cpy[strlen(command_cpy)-1] = 0;
    toml_table_t *now_character = toml_table_in(Character, command_cpy);
    if (!now_character)
    {
        fprintf(stderr, "Error: Character not found for command '%s'\n", command_cpy);
        return;
    }
    
    toml_array_t *sprites_list = toml_array_in(now_character, "sprites_list");
    if (!sprites_list) 
    {
        fprintf(stderr, "Error: Sprites list not found in character for command '%s'\n", command_cpy);
        return;
    }
    toml_array_t *sprites_png_list = toml_array_in(now_character, "sprites_png_list");
    if (!sprites_png_list) 
    {
        fprintf(stderr, "Error: Sprites list not found in character for command '%s'\n", command_cpy);
        return;
    }
    int32_t emoSize = toml_array_nelem(sprites_list);
    for (int32_t i = 0; i < emoSize; i++) 
    {
        const char* now_emo = toml_string_at(sprites_list, i).u.s;
        char* mood_copy = calloc(1024,sizeof(char));
        strcpy(mood_copy,now_emo);
        char* the_mood = calloc(1024,sizeof(char));
        strcpy(the_mood,mood);
        //printf("before the_mood = %s\n",the_mood);
        the_mood++;
        the_mood[strlen(the_mood)-1] = 0;
        //printf("the_mood = %s\n",the_mood);
        //printf("mood_copy = %s\n",mood_copy);
        if (strcmp(mood_copy, the_mood) == 0) 
        {
            ///此處要加 character.nekocat 的路徑
            const char* now_emo_path = toml_string_at(sprites_png_list, i).u.s;
            //printf("path = %s\n",now_emo_path);
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "./Assets/%s/%s", directory,now_emo_path);
            ///now_emo_path要加上./character.nekocat/image/
            SDL_Surface* character_IMG = IMG_Load(full_path);
            //printf("%s\n",now_emo);
            if (!character_IMG) 
            {
                fprintf(stderr, "Error: Unable to load image '%s': %s\n", full_path, IMG_GetError());
                continue;
            }
            //SDL_Texture* character_texture = SDL_CreateTextureFromSurface(renderer, character_IMG);
            resources->expression_texture = SDL_CreateTextureFromSurface(renderer, character_IMG);
            SDL_FreeSurface(character_IMG);
            if (!resources->expression_texture) 
            {
                fprintf(stderr, "Error: Unable to create texture from surface: %s\n", SDL_GetError());
                continue;
            }
            
            SetExpression(renderer, resources, character_start_x, character_start_y, 50, 50,255);
            //SDL_RenderCopy(renderer, character_texture, NULL, NULL);  // 渲染背景
            //printf("here\n");
            //SDL_DestroyTexture(character_texture);
            break;
        } 
    }
}

int32_t IsOverGap(toml_table_t *Player_Variable, toml_array_t* variable_array, toml_array_t* mode_array, toml_array_t* gap_array) {
    int32_t num_variables = toml_array_nelem(variable_array);
    //printf("A\n");
    if (num_variables != toml_array_nelem(mode_array) || num_variables != toml_array_nelem(gap_array))
    {
        printf("Error!\n");
        return 0;
    }
    //printf("B\n");
    for (int32_t i = 0; i < num_variables; i++) 
    {
        // 获取变量名
        
        const char* variable_name = toml_string_at(variable_array, i).u.s;
        //printf("%s\n",variable_name);
        // 获取变量的值
        toml_datum_t var_value = toml_int_in(Player_Variable, variable_name);
        //printf("C\n");
        if (!var_value.ok) {
            printf("Cannot find variable %s in Player_Variable\n", variable_name);
            return 0;
        }
        
        // 获取模式和比较值
        int mode = toml_int_at(mode_array, i).u.i;
        int64_t gap = toml_int_at(gap_array, i).u.i;
        
        //printf("%ld %ld\n",var_value.u.i,gap);
        //printf("D\n");
        // 根据模式进行比较
        switch (mode) {
            case 1: // 大于
                if (var_value.u.i <= gap) {
                    return 0;
                }
                break;
            case 2: // 等于
                if (var_value.u.i != gap) {
                    return 0;
                }
                break;
            case 3: // 小于
                if (var_value.u.i >= gap) {
                    return 0;
                }
                break;
            default:
                fprintf(stderr, "Invalid mode value %d\n", mode);
                return 0;
        }
    }
    //printf("E\n");
    return 1;
}
int32_t init_music() 
{
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // 初始化 SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 0;
    }

    return 1;
}
void initLeaveButton(RenderResources* resources, SDL_Renderer* renderer, const char* text, TTF_Font* font) {
    SDL_Color textColor = {0, 0, 0, 255};  // Black color for text
    SDL_Color bgColor = {255, 255, 255, 255};  // White color for button background
    SDL_Rect rect = {590*width_ratio, 0*height_ratio, 50*width_ratio, 50*height_ratio};  // Adjusted the size for better button appearance

    resources->leave_button = (Button*)malloc(sizeof(Button));

    // Initialize text texture
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, textColor);
    resources->leave_button->text_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    // Get text width and height
    int32_t text_width = textSurface->w *width_ratio;  // 文本寬度
    int32_t text_height = textSurface->h *height_ratio; // 文本高度
    SDL_FreeSurface(textSurface);
    
    resources->leave_button->text_rect = (SDL_Rect){rect.x + (rect.w - text_width) / 2, rect.y + (rect.h - text_height) / 2, text_width, text_height};

    // Initialize background rectangle (button background)
    SDL_Texture* button_bg_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, button_bg_texture);
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    resources->leave_button->IMG_texture = button_bg_texture;
    resources->leave_button->IMG_rect = rect;
}