#include "backpack.h"

void mychooseTextColor(uint8_t text_color, SDL_Color *titleTextColor)
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

    if (strcmp(option_x, "CENTER") == 0)
    {
        x = x - text_width / 2;
    }
    else if (strcmp(option_x, "RIGHT") == 0)
    {
        x = x - text_width;
    }

    if (strcmp(option_y, "CENTER") == 0)
    {
        y = y - text_height / 2;
    }
    else if (strcmp(option_y, "BOTTOM") == 0)
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

void myrenderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h, char *option_x, char *option_y)
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

int32_t backpackMain(backpackData *data, int32_t *status, char **name, char **description, char **image_path, backpackPath *path, backpackResources* resources, SDL_Renderer *backpack_renderer, TTF_Font* font, SDL_Window *window){

    double option = 0; //-1 = Error, 0 = Quit the game, 1 = Leave the backpack 
    int32_t window_width, window_height;
    uint8_t title_text_color = 0;

    // // printf("test 08-3\n");
    // font
    // printf("path->font_path = %s\n", path->font_path);
    // TTF_Font *font = TTF_OpenFont(path->font_path, 24);
    // // // printf("test 08-4\n");
    // if (!font)
    // {
    //     printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
    //     SDL_DestroyRenderer(backpack_renderer);
    //     SDL_DestroyWindow(window);
    //     SDL_Quit();
    //     return -1;
    // }

    // // backpack_edge_block
    // SDL_Surface *backpack_edge_block_surface = IMG_Load(path->white_edge_black_block_path);
    // if (!backpack_edge_block_surface)
    // {
    //     printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    //     SDL_DestroyRenderer(backpack_renderer);
    //     SDL_DestroyWindow(window);
    //     SDL_Quit();
    //     return -1;
    // }
    // resources->white_edge_black_block_texture = SDL_CreateTextureFromSurface(backpack_renderer, backpack_edge_block_surface);
    // SDL_FreeSurface(backpack_edge_block_surface);
    // SDL_SetTextureAlphaMod(resources->white_edge_black_block_texture, 200); // 調整透明度

    // // items
    
    // // printf("test 08-6\n");
    // for(int32_t i = 0; i < 6; i++){
    // //     printf("items[%d].status = %d\n", i, status[i]);
    //     if(status[i] == 0){
    //         continue;
    //     }
    //     char *temp;
    //     temp = (char *)malloc(sizeof(char) * 100);
    //     snprintf(temp, 100, "./item.nekocat/%s", image_path[i]);
    //     printf("temp = %s\n", temp);
    // //     printf("test 08-6-1\n");
    //     SDL_Surface *item_surface = IMG_Load(temp);
    //     //SDL_Surface *item_surface = NULL;
    // //     printf("test 08-6-2\n");
    //     if (!item_surface)
    //     {
    //         printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    //         SDL_DestroyRenderer(backpack_renderer);
    //         SDL_DestroyWindow(window);
    //         SDL_Quit();
    //         return -1;
    //     }
    // //     printf("test 08-6-3\n");
    //     resources->item_texture[i] = SDL_CreateTextureFromSurface(backpack_renderer, item_surface);
    // //     printf("test 08-6-4\n");mak
    //     SDL_FreeSurface(item_surface);
    // }

    // printf("test 08-7\n");

    // set backpack_block_renderQuads
    SDL_GetWindowSize(window, &window_width, &window_height);
    for(int32_t i = 0; i < 6; i++){
        resources->backpack_block_renderQuads[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        resources->backpack_block_renderQuads[i]->x = data->backpack_xy[i][0] * (double)window_width;
        resources->backpack_block_renderQuads[i]->y = data->backpack_xy[i][1] * (double)window_height;
        resources->backpack_block_renderQuads[i]->w = data->backpack_normal_block_width * (double)window_width;
        resources->backpack_block_renderQuads[i]->h = data->backpack_normal_block_height * (double)window_height;
        // printf("x = %d, y = %d, w = %d, h = %d\n", resources->backpack_block_renderQuads[i]->x, resources->backpack_block_renderQuads[i]->y, resources->backpack_block_renderQuads[i]->w, resources->backpack_block_renderQuads[i]->h);
    }

    // printf("test 08-8\n");

    // set item_renderQuads
    for(int32_t i = 0; i < 6; i++){
        resources->item_renderQuads[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        resources->item_renderQuads[i]->x = data->backpack_xy[i][0] * (double)window_width + data->backpack_normal_block_width * (double)window_width * 0.1;
        resources->item_renderQuads[i]->y = data->backpack_xy[i][1] * (double)window_height + data->backpack_normal_block_height * (double)window_height * 0.1;
        resources->item_renderQuads[i]->w = data->backpack_normal_block_width * (double)window_width * 0.8;
        resources->item_renderQuads[i]->h = data->backpack_normal_block_height * (double)window_height * 0.8;
        // printf("x = %d, y = %d, w = %d, h = %d\n", resources->item_renderQuads[i]->x, resources->item_renderQuads[i]->y, resources->item_renderQuads[i]->w, resources->item_renderQuads[i]->h);
    }
    
    return 0;
}

int32_t backpackItemInit(backpackItem **items, int32_t *status, char **name, char **description, char **image_path){
    
    if(items == NULL || status == NULL || name == NULL || description == NULL || image_path == NULL){
        return -1;
    }
    // printf("test 06-1\n");
    
    //IMG_Load("./item.nekocat/eraser.png");
    for (int32_t i = 0; i < 6; i++) {
        // 确保items[i]指针已经分配内存
        if (items[i] == NULL) {
            items[i] = (backpackItem *)malloc(sizeof(backpackItem));
            if (items[i] == NULL) {
                printf("Memory allocation failed for items[%d]\n", i);
                return -1;
            }
        }

        items[i]->status = status[i];
        // printf("items[%d]->status = %d\n", i, items[i]->status);

        if (items[i]->status == 0) continue;

        strncpy(items[i]->name, name[i], sizeof(items[i]->name) - 1);
        items[i]->name[sizeof(items[i]->name) - 1] = '\0'; // 确保字符串以null结尾
        // printf("items[%d]->name = %s\n", i, items[i]->name);

        strncpy(items[i]->description, description[i], sizeof(items[i]->description) - 1);
        items[i]->description[sizeof(items[i]->description) - 1] = '\0'; // 确保字符串以null结尾
        // printf("items[%d]->description = %s\n", i, items[i]->description);

        // snprintf(items[i]->image_path, sizeof(items[i]->image_path), "./item.nekocat/%s", image_path[i]);
        // // printf("items[%d]->image_path = %s\n", i, items[i]->image_path);
    }

    
    return 0;
}

void my_RenderPresentForBackpack(SDL_Renderer *renderer, backpackResources *resources, int32_t now_state, backpackData *data,  int32_t *status, char **name, char **description, char **image_path, TTF_Font *font, SDL_Window *window)
{
    if(now_state!=4)return;
    // 渲染背景
    //printf("A\n");
    if (resources->background_texture) 
    {
        //printf("background_texture is OK\n");
        if(resources->background_texture == NULL)printf("background_texture is NULL\n");
        SDL_RenderCopy(renderer, resources->background_texture, NULL, NULL);
    }

    //printf("B\n");
    int32_t window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    for(int32_t i = 0; i < 6; i++){
        int32_t block_x = data->backpack_xy[i][0] * (double)window_width;
        int32_t block_y = data->backpack_xy[i][1] * (double)window_height;
        int32_t block_w = data->backpack_normal_block_width * (double)window_width;
        int32_t block_h = data->backpack_normal_block_height * (double)window_height;
        // printf("block_x = %d, block_y = %d, block_w = %d, block_h = %d\n", block_x, block_y, block_w, block_h);
        if(status[i] == 2){
            if(resources->white_edge_black_block_texture == NULL)printf("white_edge_black_block_texture is NULL\n");
            myrenderTexture(resources->white_edge_black_block_texture, renderer, block_x, block_y, block_w, block_h, "LEFT", "TOP");
        }else{
            if(resources->backpack_block_texture == NULL)printf("backpack_block_texture is NULL\n");
            myrenderTexture(resources->backpack_block_texture, renderer, block_x, block_y, block_w, block_h, "LEFT", "TOP");
        }
        if(status[i] == 0)continue;
        int32_t center_x = block_x + block_w / 2;
        int32_t center_y = block_y + block_h / 2;
        int32_t item_w = block_w * 0.8;
        int32_t item_h = block_h * 0.8;
        // printf("center_x = %d, center_y = %d, item_w = %d, item_h = %d\n", center_x, center_y, item_w, item_h);
        myrenderTexture(resources->item_texture[i], renderer, center_x, center_y, item_w, item_h, "CENTER", "CENTER");
    }

    SDL_RenderPresent(renderer);
    return;
}

void initBackpackResources(backpackResources* resources) 
{
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
    resources->now_option_button = NULL;
    resources->leave_button = NULL;
    resources->backpack_block_texture = NULL;
    resources->white_edge_black_block_texture = NULL;
    resources->backpack_block_renderQuads = (SDL_Rect**)calloc(6, sizeof(SDL_Rect*));
    for (int i = 0; i < 6; ++i) {
        resources->backpack_block_renderQuads[i] = (SDL_Rect*)malloc(sizeof(SDL_Rect));  // 分配内存
    }
    resources->item_texture = (SDL_Texture**)malloc(6 * sizeof(SDL_Texture*));
    resources->item_renderQuads = (SDL_Rect**)calloc(6, sizeof(SDL_Rect*));
    for (int i = 0; i < 6; ++i) {
        resources->item_texture[i] = NULL;  // 分配内存 
        resources->item_renderQuads[i] = (SDL_Rect*)malloc(sizeof(SDL_Rect));  // 分配内存
    }
}
void freeBackpackResources(backpackResources* resources) 
{
    if (!resources) return;
    //printf("1\n");
    if (resources->background_texture) {
        SDL_DestroyTexture(resources->background_texture);
        resources->background_texture = NULL;
    }
    //printf("2\n");
    if (resources->dialog_box_texture) {
        SDL_DestroyTexture(resources->dialog_box_texture);
        resources->dialog_box_texture = NULL;
    }
    //printf("3\n");
    if (resources->text_texture) {
        SDL_DestroyTexture(resources->text_texture);
        resources->text_texture = NULL;
    }
    //printf("4\n");
    if (resources->character_IMG_texture) {
        for (int i = 0; i < 3; i++) {
            if (resources->character_IMG_texture[i]) {
                SDL_DestroyTexture(resources->character_IMG_texture[i]);
            }
        }
        free(resources->character_IMG_texture);
        resources->character_IMG_texture = NULL;
    }
    //printf("5\n");
    if (resources->character_IMG_renderQuads) {
        for (int i = 0; i < 3; i++) {
            if (resources->character_IMG_renderQuads[i]) {
                free(resources->character_IMG_renderQuads[i]);
            }
        }
        free(resources->character_IMG_renderQuads);
        resources->character_IMG_renderQuads = NULL;
    }
    //printf("6\n");
    if (resources->expression_texture) 
    {
        SDL_DestroyTexture(resources->expression_texture);
        resources->expression_texture = NULL;
    }
    //printf("7\n");
    if (resources->now_option_button) 
    {
        for (int i = 0; i < 3; ++i) 
        {
            if (resources->now_option_button[i]) 
            {
                free(resources->now_option_button[i]);
            }
        }
        free(resources->now_option_button);
        resources->now_option_button = NULL;
    }
    //printf("8\n");
    if (resources->leave_button) 
    {
        free(resources->leave_button);
        resources->leave_button = NULL;
    }
    //printf("9\n");
    if (resources->backpack_block_texture) 
    {
        SDL_DestroyTexture(resources->backpack_block_texture);
        resources->backpack_block_texture = NULL;
    }
    //printf("10\n");
    if (resources->white_edge_black_block_texture) 
    {
        SDL_DestroyTexture(resources->white_edge_black_block_texture);
        resources->white_edge_black_block_texture = NULL;
    }
    //printf("11\n");
    if (resources->backpack_block_renderQuads) 
    {
        for (int i = 0; i < 6; i++) 
        {
            if (resources->backpack_block_renderQuads[i]) 
            {
                free(resources->backpack_block_renderQuads[i]);
            }
        }
        free(resources->backpack_block_renderQuads);
        resources->backpack_block_renderQuads = NULL;
    }
    //printf("12\n");
    if (resources->item_texture) 
    {
        for (int i = 0; i < 6; i++) 
        {
            if (resources->item_texture[i]) 
            {
                SDL_DestroyTexture(resources->item_texture[i]);
            }
        }
        free(resources->item_texture);
        resources->item_texture = NULL;
    }
    //printf("13\n");
    if (resources->item_renderQuads) 
    {
        for (int i = 0; i < 6; i++) 
        {
            if (resources->item_renderQuads[i]) 
            {
                free(resources->item_renderQuads[i]);
            }
        }
        free(resources->item_renderQuads);
        resources->item_renderQuads = NULL;
    }
    //printf("14\n");
    return;
}