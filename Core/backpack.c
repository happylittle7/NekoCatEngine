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
    backpack_data->backpack_normal_block_width = 0.2;
    backpack_data->backpack_normal_block_height = 0.25;
    backpack_data->backpack_normal_block_gap = 0.05;
    backpack_data->backpack_choose_item_width = 0.35;

    backpack_data->backpack_xy[0][0] = 0.05;
    backpack_data->backpack_xy[0][1] = 0.1;
    backpack_data->backpack_xy[1][0] = 0.05 + 0.2 + 0.05;
    backpack_data->backpack_xy[1][1] = 0.1;
    backpack_data->backpack_xy[2][0] = 0.05;
    backpack_data->backpack_xy[2][1] = 0.1 + 0.25 + 0.05;
    backpack_data->backpack_xy[3][0] = 0.05 + 0.2 + 0.05;
    backpack_data->backpack_xy[3][1] = 0.1 + 0.25 + 0.05;
    backpack_data->backpack_xy[4][0] = 0.05;
    backpack_data->backpack_xy[4][1] = 0.1 + 0.25 + 0.05 + 0.25 + 0.05;
    backpack_data->backpack_xy[5][0] = 0.05 + 0.2 + 0.05;
    backpack_data->backpack_xy[5][1] = 0.1 + 0.25 + 0.05 + 0.25 + 0.05;

    backpack_data->choose_item_xy[0][0] = 0.6;
    backpack_data->choose_item_xy[0][1] = 0.1;
    backpack_data->backpack_choose_item_height[0] = 0.45;
    backpack_data->choose_item_xy[1][0] = 0.6;
    backpack_data->choose_item_xy[1][1] = 0.1 + 0.45 + 0.025;
    backpack_data->backpack_choose_item_height[1] = 0.1;
    backpack_data->choose_item_xy[2][0] = 0.6;
    backpack_data->choose_item_xy[2][1] = 0.1 + 0.45 + 0.025 + 0.1 + 0.025;
    backpack_data->backpack_choose_item_height[2] = 0.25;

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

int32_t backpackMain(int32_t hit, backpackData *data, int32_t *status, char **name, char **description, char **image_path, backpackPath *path, backpackResources* resources, SDL_Renderer *backpack_renderer, TTF_Font* font, SDL_Window *window){

    double option = 0; //-1 = Error, 0 = Quit the game, 1 = Leave the backpack 
    int32_t window_width, window_height;
    uint8_t title_text_color = 0;

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
    
    // random choose item
    if(choose_item == -1){
        srand((unsigned)time(NULL));
        choose_item = rand() % now_own_item;
    }

    if(hit == 1){
        int32_t mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        for(int32_t i = 0; i < 6; i++){
            int32_t block_x = data->backpack_xy[i][0] * (double)window_width;
            int32_t block_y = data->backpack_xy[i][1] * (double)window_height;
            int32_t block_w = data->backpack_normal_block_width * (double)window_width;
            int32_t block_h = data->backpack_normal_block_height * (double)window_height;
            if(myjudgeButtonPressed(mouse_x, mouse_y, block_x, block_y, block_w, block_h, "LEFT", "TOP")){
                if(status[i] == 1){
                    status[choose_item] = 1;
                    status[i] = 2;
                    choose_item = i;
                }
                break;
            }
        }
    }
    status[choose_item] = 2;
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

void my_RenderPresentForBackpack(SDL_Renderer *renderer, backpackResources *resources, int32_t now_state, backpackData *data, backpackPath* path, int32_t *status, char **name, char **description, char **image_path, TTF_Font *font, SDL_Window *window)
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

    for(int32_t i = 0; i < 3; i++){
        int32_t block_x = data->choose_item_xy[i][0] * (double)window_width;
        int32_t block_y = data->choose_item_xy[i][1] * (double)window_height;
        int32_t block_w = data->backpack_choose_item_width * (double)window_width;
        int32_t block_h = data->backpack_choose_item_height[i] * (double)window_height;

        myrenderTexture(resources->backpack_block_texture, renderer, block_x, block_y, block_w, block_h, "LEFT", "TOP");
        TTF_Font *temp_font;
        switch(i){
            case 0:
                block_x = block_x + block_w / 2;
                block_y = block_y + block_h / 2;
                block_w = block_w * 0.8;
                block_h = block_h * 0.8;
                myrenderTexture(resources->item_texture[choose_item], renderer, block_x, block_y, block_w, block_h, "CENTER", "CENTER");
                break;
            case 1:
                temp_font = TTF_OpenFont(path->font_path, 50);
                if(!temp_font){
                    printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return;
                }
                myDisplayText(renderer, temp_font, name[choose_item], (SDL_Color){255, 255, 255}, block_x + block_w / 2, block_y + block_h / 2, "CENTER", "CENTER");
                TTF_CloseFont(temp_font);
                break;
            case 2:
                temp_font = TTF_OpenFont(path->font_path, 30);
                if(!temp_font){
                    printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return;
                }
                block_x = block_x + block_w * 0.03;
                block_y = block_y + block_h * 0.1;
                block_w = block_w * 0.8;

                printf("description = %s\n", description[choose_item]);
                multipleLineDialogText(renderer, resources, temp_font, &description[choose_item], (SDL_Color){255, 255, 255}, &block_x, &block_y, block_w);
                SDL_RenderCopy(renderer, resources->text_texture, NULL, &resources->text_renderQuad);
                TTF_CloseFont(temp_font);
                break;
        }
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

void multipleLineDialogText(SDL_Renderer* renderer, backpackResources *resources, TTF_Font* font, char** text, SDL_Color textColor, int32_t* x, int32_t* y, int32_t max_w){
    SDL_Surface* textSurface;
    //SDL_Texture* textTexture;
    printf("text = %s\n", (*text));
    char *print_char = (*text);
    textSurface = TTF_RenderUTF8_Blended_Wrapped(font, print_char, textColor,625);
    resources->text_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureAlphaMod(resources->text_texture, 255);
    int32_t text_width = textSurface->w;  // 文本寬度
    int32_t text_height = textSurface->h; // 文本高度
    int32_t now_x = *x;
    int32_t now_y = *y;
    /*
    if (now_x >= max_w)
    {
        now_y += text_height;
        now_x = *x;
    }
    */
    printf("width = %d, height = %d\n", text_width, text_height);
    SDL_Rect renderQuad = { now_x, now_y, text_width, text_height };  // 定義渲染區域
    // SDL_Rect renderQuad = { now_x*width_ratio, now_y*height_ratio, text_width*width_ratio, text_height*height_ratio };  // 定義渲染區域
    //resources->text_texture = textTexture;
    resources->text_renderQuad.x = renderQuad.x;
    resources->text_renderQuad.y = renderQuad.y;
    resources->text_renderQuad.w = renderQuad.w;
    resources->text_renderQuad.h = renderQuad.h;
    //printf("x = %d, y = %d\n", renderQuad.x, renderQuad.y);
    printf("x = %d, y = %d\n", renderQuad.x, renderQuad.y);
    //SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);  // 執行渲染操作
    
    //SDL_DestroyTexture(textTexture);  // 銷毀紋理資源
    SDL_FreeSurface(textSurface);  // 釋放表面資源

    return;
}

uint8_t myjudgeButtonPressed(int32_t x, int32_t y, int32_t button_x, int32_t button_y, int32_t normal_button_width, int32_t normal_button_height, char *option_x, char *option_y){
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