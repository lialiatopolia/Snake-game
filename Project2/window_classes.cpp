/*
� ������ ����� �������� ����������� ������� Screen, Menu, Menu_Start, Menu_Info,
Menu_Exit, Input_Window, � �������� ������������ �������� ���������� ���� ����������.
*/

#include "const.h"
#include "record.h"
#include "window_classes.h"
#include <string>

/* ������� ��� ������ ��������� �� �����������:
surface - �����������, �� ������� ����� ���������� �����;
message - �����, ������� ����� ����������;
font - ���� � ������;
size - ������ ������;
color - ���� ������;
rect - ������������� ������� �� ����������� surface, � ������� ����� ���������� �����;
centered - ����� ������� ������������ rect �� surface. */
void print_ttf(SDL_Surface* surface, const std::string& message, const char* font, int size, SDL_Color color, SDL_Rect* rect, Centered centered) {
    TTF_Font* ttf_font = TTF_OpenFont(font, size);
    if (!ttf_font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* text_surface = TTF_RenderUTF8_Blended(ttf_font, message.c_str(), color);
    if (!text_surface) {
        std::cerr << "TTF_RenderUTF8_Blended Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(ttf_font);
        return;
    }

    if (centered == x_centered) {
        rect->x = (surface->w - text_surface->w) / 2;
    }
    else if (centered == y_centered) {
        rect->y = (surface->h - text_surface->h) / 2;
    }
    else if (centered == xy_centered) {
        rect->x = (surface->w - text_surface->w) / 2;
        rect->y = (surface->h - text_surface->h) / 2;
    }

    SDL_BlitSurface(text_surface, NULL, surface, rect);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(ttf_font);
}

// ����������� ������ Screen
Screen::Screen()
{
    x = y = 0;
    height = HEIGHT; // ������ ������
    width = WIDTH; // ������ ������
    page = welcome_page; // ��������� ��������, ������������ �� ������
}

// ���������� ������ Screen
Screen::~Screen()
{
    SDL_DestroyTexture(texture_end);
    SDL_DestroyTexture(texture_game);
    SDL_DestroyTexture(texture_info);
    SDL_DestroyTexture(texture_welcome);
}

// ����� ������ Screen ��� ����������� ������ �������� ������ �� ��������
void Screen::apply_texture(SDL_Renderer* renderer, std::vector<Record*>* records)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    switch (page)
    {
    case end_page:
    {
        if (was_created == false)
        {
            // ���� �������� ��� ������ ����� ��������� �� ���� ��� �������
            create_end_texture(renderer, records);
            was_created = true;
        }        
        // ���������� ��������
        SDL_RenderCopy(renderer, texture_end, NULL, &rect);
        break;
    }
    case game_page:
    {
        // ����� �������� ����� �� ����� ����
        SDL_RenderCopy(renderer, texture_game, NULL, &rect);
        break;
    }

    case info_page:
    {
        // ����� �������� ����� ��� ������ ������� �� ����
        SDL_RenderCopy(renderer, texture_info, NULL, &rect);
        break;
    }
    case welcome_page:
    {
        // ����� �������� ��������� ��������
        SDL_RenderCopy(renderer, texture_welcome, NULL, &rect);
        break;
    }
    }
}

// ����� ������ Screen ������� �������� ��� ������ ����� ���������
// ���������� std::to_string ��� �������������� ����� � ������
std::string format_highscore(int position, const std::string& name, int score) {
    return std::to_string(position) + ". " + name + " - " + std::to_string(score);
}

//  �������� ��������� ������ ���� ���
void Screen::create_end_texture(SDL_Renderer* renderer, std::vector<Record*>* records) {
    if (texture_end) {
        SDL_DestroyTexture(texture_end);
    }

    selection_sort(records);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    if (!surface) {
        std::cerr << "SDL_CreateRGBSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));

    SDL_Rect rect = { 0, 150, 0, 0 };
    print_ttf(surface, "Game Over!", FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);

    rect.y += 50;
    print_ttf(surface, "Highscores:", FONT2, FONT_TITLE_SIZE - 10, BLACK, &rect, x_centered);

    for (size_t i = 0; i < std::min(records->size(), size_t(RECORDS_MAX_NUM)); ++i) {
        const auto& record = records->at(i);
        rect.y += 30;
        SDL_Color color = (record == records->back()) ? BLACK : DIM_GRAY;
        print_ttf(surface, format_highscore(i + 1, record->name, record->length), FONT2, FONT_TEXT_SIZE, color, &rect, x_centered);
    }

    texture_end = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
// ����� ������ Screen ������� ��� ����������� �������� ��� ������
void Screen::create_textures(SDL_Renderer* renderer)
{
    std::string text; // �����, ������� ����� ���������� �� ������
    SDL_Rect rect;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // ������� �������� ��� ����������� ��� ���������� ����
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    text = "Game Over!";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    texture_end = SDL_CreateTextureFromSurface(renderer, surface);
    // ������� �������� ��� ����������� �� ����� ����
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    texture_game = SDL_CreateTextureFromSurface(renderer, surface);
    // ������� �������� ��� ����������� ��� ������ ������� �� ����
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    text = "Tiutiukina VO731b";
    rect.x = 0;
    rect.y = 20;
    print_ttf(surface, text, FONT2, FONT_TEXT_SIZE, BLACK, &rect, x_centered);
    text = "Instructions";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    text = "To move the snake";
    rect.x = 0;
    rect.y = 200;
    print_ttf(surface, text, FONT2, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    text = "use keys: Up, Down,";
    rect.x = 0;
    rect.y = 230;
    print_ttf(surface, text, FONT2, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    text = "Left and Right";
    rect.x = 0;
    rect.y = 260;
    print_ttf(surface, text, FONT2, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    texture_info = SDL_CreateTextureFromSurface(renderer, surface);



    // ������� �������� ��� ����������� ��� ������� ����������
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    // ������� ����������� ����� �� ��������� ��������
    text = "Snake game!";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    text = "Enter your name to start the game:";
    rect.x = 0;
    rect.y = 200;
    print_ttf(surface, text, FONT2, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    texture_welcome = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// ����������� ������ Menu
Menu::Menu()
{
    x = 0;
    y = HEIGHT - MENU_HEIGHT;
    height = MENU_HEIGHT; // ������ ������ ����
    width = WIDTH / MENU_ITEM_NUM; // ������ ������ ����
}

// ���������� ������ Menu
Menu::~Menu()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_clicked);
    SDL_DestroyTexture(texture_hovered);
}

// ����� ������ Menu ��� ����������� ������ �������� ������ ���� �� ��������
void Menu::apply_texture(SDL_Renderer* renderer)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    // ���������� ��������
    switch (state)
    {
    case clicked:
    {
        // ���� �� ������ ���� �������� ������
        SDL_RenderCopy(renderer, texture_clicked, NULL, &rect);
        break;
    }
    case hovered:
    {
        // ���� �� ����� ���� ������ �����
        SDL_RenderCopy(renderer, texture_hovered, NULL, &rect);
        break;
    }
    case usual:
    {
        // ���� ����� ���� � ������� ���������
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        break;
    }
    }
}

// ����� ������ Menu ������� ��� ����������� �������� ��� ������ ����
void Menu::create_textures(SDL_Renderer* renderer, std::string& text)
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // ������� �������� ��� ����������� ������ ���� � ������� ���������
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, MENU_USUAL_COLOR.r, MENU_USUAL_COLOR.g, MENU_USUAL_COLOR.b));
    print_ttf(surface, text, (char*)FONT, FONT_MENU_SIZE, DIM_GRAY, &rect, xy_centered);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    // ������� �������� ��� ����������� ������ ����, ����� �� ���� �������� ������
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, MENU_CLICKED_COLOR.r, MENU_CLICKED_COLOR.g, MENU_CLICKED_COLOR.b));
    print_ttf(surface, text, (char*)FONT, FONT_MENU_SIZE, WHITE, &rect, xy_centered);
    texture_clicked = SDL_CreateTextureFromSurface(renderer, surface);
    // ������� �������� ��� ����������� ������ ����, ����� �� ���� ������ ������
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, MENU_HOVERED_COLOR.r, MENU_HOVERED_COLOR.g, MENU_HOVERED_COLOR.b));
    print_ttf(surface, text, (char*)FONT, FONT_MENU_SIZE, BLACK, &rect, xy_centered);
    texture_hovered = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// ����� ������ Menu ����������, ��������� �� ����� ������ ������ ����
bool Menu::is_in(Sint32 x, Sint32 y)
{
    if (this->x < x && x < this->x + width && this->y < y && y < this->y + height)
    {
        // ����� � ������������ x, y ��������� ������ ������ ����
        return true;
    }
    return false;
}

// ����� ������ Menu ������ ��������� ������ ����
void Menu::set_position(Menu* menu)
{
    x = menu->x + menu->width;
    y = HEIGHT - MENU_HEIGHT;
}

// ����������� ������ Menu_Exit
Menu_Exit::Menu_Exit() : Menu()
{
}

// ����� ������ Menu_Exit ���������� ��������, ����� �������� ������ �� ������ ���� �����
bool Menu_Exit::down(bool*, Screen*, Game_Field*, Snake*, Food*, SDL_Renderer*)
{
    state = clicked; // ������������� ������� ��������� ������ ����
    return true; // �������� true ��������, ��� ����� ����� �� ����������
}

// ����������� ������ Menu_Info
Menu_Info::Menu_Info() : Menu()
{
}

// ����� ������ Menu_Info ���������� ��������, ����� �������� ������ �� ������ ���� �������
bool Menu_Info::down(bool* is_playing, Screen* screen, Game_Field*, Snake*, Food*, SDL_Renderer*)
{
    state = clicked; // ������������� ������� ��������� ������ ����
    *is_playing = false; // �������� false ��������, ��� ����� ��������� ����, ���� ��� ��������
    screen->page = info_page; // ����� ���������� �� ������ ���������� �������� �� ��������
    return false; // �������� false ��������, ��� ���������� ���������� ��������
}

// ����������� ������ Menu_Start
Menu_Start::Menu_Start() : Menu()
{
}

// ����� ������ Menu_Start ���������� ��������, ����� �������� ������ �� ������ ���� �����
bool Menu_Start::down(bool* is_playing, Screen* screen, Game_Field* game_field, Snake* snake, Food* food, SDL_Renderer* renderer)
{
    state = clicked; // ������������� ������� ��������� ������ ����
    *is_playing = true; // �������� true ��������, ��� ����� ������ ����
    screen->page = game_page; // ����� ���������� �� ������ ���������� �������� � ������� �����
    game_field->create_field(renderer); // ������� ������� ����
    snake->start(game_field); // ������ ������
    food->set_position(snake, game_field); // ������ ��������� ���
    return false; // �������� false ��������, ��� ���������� ���������� ��������
}

// ����������� ������ Input_Window ��� ���� �����
Input_Window::Input_Window()
{
    is_displayed = true; // �� ��������� ���� ����� ������������
    state = usual; // ���� ����� � ������� ���������
    height = INPUT_WINDOW_HEIGHT; // ������ ���� �����
    width = INPUT_WINDOW_WIDTH; // ������ ���� �����
    x = (WIDTH - INPUT_WINDOW_WIDTH) / 2; // ���������� x �������� ������ ���� ���� �����
    y = INPUT_WINDOW_Y; // ���������� y �������� ������ ���� ���� �����
}

// ����� ������ Input_window ��� ����������� ������ �������� ���� ����� �� ��������
void Input_Window::apply_texture(SDL_Renderer* renderer)
{
    if (!is_displayed)
    {
        // ���� ���� ����� �� ������������ �� ������
        return;
    }
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // ���������� ��������
    switch (state)
    {
    case clicked:
    {
        // ���� �� ���� ����� �������� ������
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, INPUT_CLICKED_COLOR.r, INPUT_CLICKED_COLOR.g, INPUT_CLICKED_COLOR.b));
        if (text.size())
        {
            print_ttf(surface, text, (char*)FONT, FONT_TEXT_SIZE, BLACK, &rect, y_centered);
        }
        break;
    }
    case hovered:
    {
        // ���� �� ���� ����� ������ �����
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, INPUT_HOVERED_COLOR.r, INPUT_HOVERED_COLOR.g, INPUT_HOVERED_COLOR.b));
        if (text.size())
        {
            print_ttf(surface, text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, y_centered);
        }
        break;
    }
    case usual:
    {
        // ���� ���� ����� � ������� ���������
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, INPUT_USUAL_COLOR.r, INPUT_USUAL_COLOR.g, INPUT_USUAL_COLOR.b));
        if (text.size())
        {
            print_ttf(surface, text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, y_centered);
        }
        break;
    }
    }
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

// ����� ������ Input_Window ����������, ��������� �� ����� ������ ���� �����
bool Input_Window::is_in(Sint32 x, Sint32 y)
{
    if (is_displayed && this->x < x && x < this->x + width && this->y < y && y < this->y + height)
    {
        // ����� � ������������ x, y ��������� ������ ���� �����
        return true;
    }
    return false;
}