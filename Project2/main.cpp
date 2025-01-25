#include <iostream>
#include <cstdlib> // ��� ������� rand() � srand()
#include <ctime> // ��� ������� time()
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "const.h"
#include "game_classes.h"
#include "window_classes.h"

// ���������� ����������
bool is_playing = false; // ���� true, �� ������������ ������, ����� false
bool is_paused = false; // ���� true, ���� �� �����
std::vector<Record*> all_records; // ������ � ��������� � ����
Screen screen; // ����� ����������
Menu_Exit menu_exit; // ����� ���� �����
Menu_Info menu_info; // ����� ���� ����
Menu_Start menu_start; // ����� ���� �����
Input_Window input_window; // ���� �����
Game_Field game_field; // ������� ����
Snake snake; // ������
Food food; // ���
TTF_Font* score_font = nullptr; // ����� ��� ������ �����
SDL_Texture* score_texture = nullptr; // �������� ��� ������

void draw_score(SDL_Renderer* renderer, int score)
{
    static int last_score = -1;
    static SDL_Texture* score_texture = nullptr;

    if (score != last_score) {
        last_score = score;
        if (score_texture) {
            SDL_DestroyTexture(score_texture);
        }

        std::string score_text = "Score: " + std::to_string(score);
        SDL_Surface* surface = TTF_RenderText_Solid(score_font, score_text.c_str(), INPUT_CLICKED_COLOR);
        score_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect dest_rect = { WIDTH - 200, 50, 150, 50 };
    SDL_RenderCopy(renderer, score_texture, nullptr, &dest_rect);
}




// ������� ������� ����������� � ���� ����������
void draw(SDL_Renderer* renderer)
{
    //SDL_RenderClear(renderer); // ������� ��������
    screen.apply_texture(renderer, &all_records); // ������ �����
    menu_start.apply_texture(renderer); // ������ ����� ���� �����
    menu_info.apply_texture(renderer); // ������ ����� ���� ����
    menu_exit.apply_texture(renderer); // ������ ����� ���� �����
    input_window.apply_texture(renderer); // ������ ���� �����
    if (is_playing)
    {
        // ���� �������� ����
        game_field.apply_texture(renderer); // ������ ������� ����
        snake.apply_texture(renderer, &game_field); // ������ ������
        food.apply_texture(renderer, &game_field); // ������ ���
        draw_score(renderer, snake.n-1);
    }
    // ������� ����������� �����
    SDL_RenderPresent(renderer);
}

struct TimerData {
    SDL_Renderer* renderer;
    float speed_factor;
};

/* ������� ���������� ��������, ������������� ��� �������� ��������:
interval - ���������� �������, ����� ������� ���������� �������;
param - ��������, �� ������� ��������� ��������� ������ ��� ����. */
Uint32 game_timer_callback(Uint32 interval, void* param)
{
    TimerData* data = static_cast<TimerData*>(param);
    SDL_Renderer* renderer = data->renderer;
    if (!is_playing) {
        return 0; // ���� ���� ��������, ��������� ������
    }
    if (is_playing)
    {
        if (!snake.control(&food) ||
            !game_field.available_position(&snake) ||
            !food.was_eaten(&snake, &game_field))
        {
            is_playing = false;
            screen.page = end_page;
            write_record(&all_records, &input_window.text, snake.n - 1);
            return 0;
        }
        draw(renderer);
        return interval;
    }
    else if (is_paused) {
        return interval;
    }

    // ��������� ������� �� ���������, ����� �������� ��������������
    return interval;
}

int main(int argc, char *argv[])
{
    // ������������� �������� ��������� ����� � �������� ���������� ����� ��� ��������� ��������� �����
    srand(static_cast<unsigned int>(time(0)));
    // �������������� ��� SDL ����������
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // �������������� TTL ��� ������ ������ �� �����
    if (TTF_Init() != 0)
    {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    // ������� ���� ����������
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // ������� �������� ��� ��������� �� ������
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // ������� ��� �������� ��� ������
    screen.create_textures(renderer);
    // ������� ��� �������� ��� ������ ���� �����
    std::string menu_name = "Start";
    menu_start.create_textures(renderer, menu_name);
    // ������� ��� �������� ��� ������ ���� �������
    menu_info.set_position(&menu_start);
    menu_name = "Info";
    menu_info.create_textures(renderer, menu_name);
    // ������� ��� �������� ��� ������ ���� �����
    menu_exit.set_position(&menu_info);
    menu_name = "Exit";
    menu_exit.create_textures(renderer, menu_name);
    // ������� ����������� �� �����
    draw(renderer);

    score_font = TTF_OpenFont(FONT2, 16); 
    if (!score_font)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    //
    std::string name1 = "Polina";
    Record record1(&name1, 20);
    name1 = "Polya";
    Record record2(&name1, 30);
    name1 = "Polly";
    Record record3(&name1, 20);
    bool result_bool = record1 == record2;
    std::cout << record1.name << "==" << record2.name << "=" << result_bool << std::endl;
    result_bool = record1 == record3;
    std::cout << record1.name << "==" << record3.name << "=" << result_bool << std::endl;

    //
    SDL_TimerID my_timer;
    float speed_factor = 1.0f;

    TimerData timer_data = { renderer, speed_factor };//
    SDL_StopTextInput(); // ������������� ��������� ������� ���� SDL_TEXTINPUT - ���� ������
    // ������ ��������� ����� � ��������� ����
    read_file(&all_records);
    // ���� ��������� �������
    SDL_Event window_event; // ������� � ���� ����������
    bool quit = false; // �������� �������� �� ����� �� �����: ���� true, �� ���� ��������� ���������� ���������
    while (!quit)
    {
        if (SDL_PollEvent(&window_event))
        {
            switch (window_event.type)
            {
            case SDL_QUIT:
            {
                // ���� ������������ ������ ����
                quit = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                // ���� ������������ ����� ������� �� ����������
                if (is_playing)
                {
                    // ���� �������� ����, ��������� ����� ����� ������ �������: ����, �����, ������ � �����
                    switch (window_event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_DOWN:
                    {
                        // ������ ������� ����
                        snake.change_direction(down);
                        break;
                    }
                    case SDL_SCANCODE_LEFT:
                    {
                        // ������ ������� �����
                        snake.change_direction(left);
                        break;
                    }
                    case SDL_SCANCODE_RIGHT:
                    {
                        // ������ ������� ������
                        snake.change_direction(right);
                        break;
                    }
                    case SDL_SCANCODE_UP:
                    {
                        // ������ ������� �����
                        snake.change_direction(up);
                        break;
                    }
                    case SDL_SCANCODE_SPACE:
                    {
                        is_paused = !is_paused;

                        if (is_paused) {
                            // ���� ����� ��������, ������� ���������� ������
                            SDL_RemoveTimer(my_timer);
                        }
                        else {
                            // ���� ����� ���������, ������������ ������
                            Uint32 time_interval = GAME_TIME_INTERVAL;
                            my_timer = SDL_AddTimer(time_interval, game_timer_callback, &timer_data);
                        }

                        // ��������� ����� � ����� ������
                        draw(renderer);
                        break;
                    }
                    }
                }
                if (window_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    // ������ ������� Enter ��� ������ ����
                    quit = menu_start.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                    input_window.is_displayed = false; // ���� ����� ������ �� ������������
                    Uint32 time_interval = GAME_TIME_INTERVAL; // �������� ����� �������� ������� game_timer_callback
                    my_timer = SDL_AddTimer(time_interval, game_timer_callback, &timer_data);
                    screen.was_created = false;
                }
                if (input_window.state == clicked && window_event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && input_window.text.size())
                {
                    // ���� ���� ������ ������������ � �������, � ������������ �������� ������� Backspace (�������)
                    input_window.text.pop_back();
                }                
                break;
            }
            case SDL_TEXTINPUT:
            {
                // ���� ������ � ���� �����
                if (input_window.text.size() < NAME_MAX_LENGTH)
                {
                    // ���� ����� ������-�����, ���������� ������� ������ ������������ ���������� ����� �����, �� ���� ������������
                    if (window_event.text.text[0] == ' ')
                    {
                        // � �����, �������� �������������, ��������� ������������ �������
                        input_window.text += "_";
                    }
                    else
                    {
                        input_window.text += window_event.text.text;
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                // ���� ������������ ������� ������� ����
                if (window_event.button.button == SDL_BUTTON_LEFT)
                {
                    // ���� ������������ ������� ����� ������� ����
                    Sint32 x = window_event.button.x;
                    Sint32 y = window_event.button.y;
                    // ������ ����
                    if (menu_start.is_in(x, y))
                    {
                        // ���� ������������ ������� �� ������ ����
                        quit = menu_start.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false; // ���� ����� ������ �� ������������
                        // ������� ������
                        Uint32 time_interval = GAME_TIME_INTERVAL; // �������� ����� �������� ������� game_timer_callback
                        my_timer = SDL_AddTimer(time_interval, game_timer_callback, &timer_data);
                        screen.was_created = false;
                    }
                    else if (menu_info.is_in(x, y))
                    {
                        // ���� ������������ ������� �� ������ ����
                        quit = menu_info.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false; // ���� ����� ������ �� ������������
                    }
                    else if (menu_exit.is_in(x, y))
                    {
                        // ���� ������������ ������� �� ������ ����
                        quit = menu_exit.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false; // ���� ����� ������ �� ������������
                    }
                    // ���� �����
                    if (input_window.is_in(x, y))
                    {
                        // ���� ������������ ������� �� ���� �����
                        input_window.state = clicked;
                        SDL_StartTextInput();
                    }
                    else if (input_window.is_displayed)
                    {
                        // ���� ������������ ������� �� ������� �������
                        input_window.state = usual;
                        SDL_StopTextInput();
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                // ���� ������������ �������� ������ ����
                if (window_event.button.button == SDL_BUTTON_LEFT)
                {
                    // ���� ������������ �������� ����� ������ ����
                    if (menu_start.state == clicked)
                    {
                        // ���� ������������ �������� ������ ���� ����� ����, ��� ������� �� ������ ����
                        menu_start.state = usual;
                    }
                    else if (menu_info.state == clicked)
                    {
                        // ���� ������������ �������� ������ ���� ����� ����, ��� ������� �� ������ ����
                        menu_info.state = usual;
                    }
                    else if (menu_exit.state == clicked)
                    {
                        // ���� ������������ �������� ������ ���� ����� ����, ��� ������� �� ������ ����
                        menu_exit.state = usual;
                    }
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                // ���� ������������ �����
                Sint32 x = window_event.motion.x;
                Sint32 y = window_event.motion.y;
                // ������ ����
                if (menu_start.state == hovered && !menu_start.is_in(x, y))
                {
                    // ���� ������������ ����� ����� � ������ ����
                    menu_start.state = usual;
                }
                else if (menu_start.state == !clicked && menu_start.is_in(x, y))
                {
                    // ���� ������������ ����� ����� �� ����� ����
                    menu_start.state = hovered;
                }
                if (menu_info.state == hovered && !menu_info.is_in(x, y))
                {
                    // ���� ������������ ����� ����� � ������ ����
                    menu_info.state = usual;
                }
                else if (menu_info.state == !clicked && menu_info.is_in(x, y))
                {
                    // ���� ������������ ����� ����� �� ����� ����
                    menu_info.state = hovered;
                }
                if (menu_exit.state == hovered && !menu_exit.is_in(x, y))
                {
                    // ���� ������������ ����� ����� � ������ ����
                    menu_exit.state = usual;
                }
                else if (menu_exit.state == !clicked && menu_exit.is_in(x, y))
                {
                    // ���� ������������ ����� ����� �� ����� ����
                    menu_exit.state = hovered;
                }
                // ���� �����
                if (input_window.is_in(x, y) && input_window.state != clicked)
                {
                    // ���� ������������ ����� ����� �� ���� �����
                    input_window.state = hovered;
                }
                else if (!input_window.is_in(x, y) && input_window.state == hovered)
                {
                    // ���� ������������ ����� ����� �� ���� �����
                    input_window.state = usual;
                }
            }
            }
            draw(renderer);
        }
    }
    // ����������� �� ������
    SDL_DestroyTexture(score_texture);
    TTF_CloseFont(score_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}