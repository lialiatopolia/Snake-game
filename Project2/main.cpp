#include <iostream>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "const.h"
#include "game_classes.h"
#include "window_classes.h"

// Глобальные переменные
bool is_playing = false; // если true, то пользователь играет, иначе false
bool is_paused = false; // если true, игра на паузе
std::vector<Record*> all_records; // вектор с рекордами в игре
Screen screen; // экран приложения
Menu_Exit menu_exit; // пункт меню Выход
Menu_Info menu_info; // пункт меню Инфо
Menu_Start menu_start; // пункт меню Старт
Input_Window input_window; // окно ввода
Game_Field game_field; // игровое поле
Snake snake; // змейка
Food food; // еда
TTF_Font* score_font = nullptr; // Шрифт для текста очков
SDL_Texture* score_texture = nullptr; // Текстура для текста

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




// Функция выводит изображения в окно приложения
void draw(SDL_Renderer* renderer)
{
    //SDL_RenderClear(renderer); // очищаем рендерер
    screen.apply_texture(renderer, &all_records); // рисуем экран
    menu_start.apply_texture(renderer); // рисуем пункт меню Старт
    menu_info.apply_texture(renderer); // рисуем пункт меню Инфо
    menu_exit.apply_texture(renderer); // рисуем пункт меню Выход
    input_window.apply_texture(renderer); // рисуем окно ввода
    if (is_playing)
    {
        // Если играется игра
        game_field.apply_texture(renderer); // рисуем игровое поле
        snake.apply_texture(renderer, &game_field); // рисуем змейку
        food.apply_texture(renderer, &game_field); // рисуем еду
        draw_score(renderer, snake.n-1);
    }
    // Покажем обновленный экран
    SDL_RenderPresent(renderer);
}

struct TimerData {
    SDL_Renderer* renderer;
    float speed_factor;
};

/* Функция вызывается таймером, установленным для игрового процесса:
interval - промежуток времени, через которые вызывается функция;
param - рендерер, на который выводится изменения экрана при игре. */
Uint32 game_timer_callback(Uint32 interval, void* param)
{
    TimerData* data = static_cast<TimerData*>(param);
    SDL_Renderer* renderer = data->renderer;
    if (!is_playing) {
        return 0; // Если игра окончена, завершаем таймер
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

    // Добавляем возврат по умолчанию, чтобы избежать предупреждения
    return interval;
}

int main(int argc, char *argv[])
{
    // Устанавливаем значение системных часов в качестве стартового числа для генерации случайных чисел
    srand(static_cast<unsigned int>(time(0)));
    // Инициализируем все SDL подсистемы
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Инициализируем TTL для вывода текста на экран
    if (TTF_Init() != 0)
    {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    // Создаем окно приложения
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Создаем рендерер для рисования на экране
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Создаем все текстуры для экрана
    screen.create_textures(renderer);
    // Создаем все текстуры для пункта меню Старт
    std::string menu_name = "Start";
    menu_start.create_textures(renderer, menu_name);
    // Создаем все текстуры для пункта меню Справка
    menu_info.set_position(&menu_start);
    menu_name = "Info";
    menu_info.create_textures(renderer, menu_name);
    // Создаем все текстуры для пункта меню Выход
    menu_exit.set_position(&menu_info);
    menu_name = "Exit";
    menu_exit.create_textures(renderer, menu_name);
    // Выводим изображение на экран
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
    SDL_StopTextInput(); // останавливаем обработку событий окна SDL_TEXTINPUT - ввод текста
    // Чтение бинарного файла с рекордами игры
    read_file(&all_records);
    // Цикл обработки событий
    SDL_Event window_event; // события в окне приложения
    bool quit = false; // параметр отвечает за выход из цикла: если true, то пора завершать выполнение программы
    while (!quit)
    {
        if (SDL_PollEvent(&window_event))
        {
            switch (window_event.type)
            {
            case SDL_QUIT:
            {
                // Если пользователь закрыл окно
                quit = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                // Если пользователь нажал клавишу на клавиатуре
                if (is_playing)
                {
                    // Если играется игра, проверить нужно всего четыре клавиши: Вниз, Влево, Вправо и Вверх
                    switch (window_event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_DOWN:
                    {
                        // Нажата клавиша Вниз
                        snake.change_direction(down);
                        break;
                    }
                    case SDL_SCANCODE_LEFT:
                    {
                        // Нажата клавиша Влево
                        snake.change_direction(left);
                        break;
                    }
                    case SDL_SCANCODE_RIGHT:
                    {
                        // Нажата клавиша Вправо
                        snake.change_direction(right);
                        break;
                    }
                    case SDL_SCANCODE_UP:
                    {
                        // Нажата клавиша Вверх
                        snake.change_direction(up);
                        break;
                    }
                    case SDL_SCANCODE_SPACE:
                    {
                        is_paused = !is_paused;

                        if (is_paused) {
                            // Если пауза включена, убираем обновления экрана
                            SDL_RemoveTimer(my_timer);
                        }
                        else {
                            // Если пауза выключена, возобновляем таймер
                            Uint32 time_interval = GAME_TIME_INTERVAL;
                            my_timer = SDL_AddTimer(time_interval, game_timer_callback, &timer_data);
                        }

                        // Обновляем экран в любом случае
                        draw(renderer);
                        break;
                    }
                    }
                }
                if (window_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    // Нажата клавиша Enter для начала игры
                    quit = menu_start.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                    input_window.is_displayed = false; // окно ввода больше не отображается
                    Uint32 time_interval = GAME_TIME_INTERVAL; // интервал между вызовами функции game_timer_callback
                    my_timer = SDL_AddTimer(time_interval, game_timer_callback, &timer_data);
                    screen.was_created = false;
                }
                if (input_window.state == clicked && window_event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && input_window.text.size())
                {
                    // Если окно вывода отображается и активно, а пользователь нажимает клавишу Backspace (удалить)
                    input_window.text.pop_back();
                }                
                break;
            }
            case SDL_TEXTINPUT:
            {
                // Ввод текста в окно ввода
                if (input_window.text.size() < NAME_MAX_LENGTH)
                {
                    // Если длина тексте-имени, введенного игроком меньше максимальной допустимой длины имени, то ввод продолжается
                    if (window_event.text.text[0] == ' ')
                    {
                        // В имени, вводимом пользователем, запрещено использовать пробелы
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
                // Если пользователь кликнул кнопкой мыши
                if (window_event.button.button == SDL_BUTTON_LEFT)
                {
                    // Если пользователь кликнул левой кнопкой мыши
                    Sint32 x = window_event.button.x;
                    Sint32 y = window_event.button.y;
                    // Пункты меню
                    if (menu_start.is_in(x, y))
                    {
                        // Если пользователь кликнул по пункту меню
                        quit = menu_start.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false; // окно ввода больше не отображается
                        // Создаем таймер
                        Uint32 time_interval = GAME_TIME_INTERVAL; // интервал между вызовами функции game_timer_callback
                        my_timer = SDL_AddTimer(time_interval, game_timer_callback, &timer_data);
                        screen.was_created = false;
                    }
                    else if (menu_info.is_in(x, y))
                    {
                        // Если пользователь кликнул по пункту меню
                        quit = menu_info.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false; // окно ввода больше не отображается
                    }
                    else if (menu_exit.is_in(x, y))
                    {
                        // Если пользователь кликнул по пункту меню
                        quit = menu_exit.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false; // окно ввода больше не отображается
                    }
                    // Окно ввода
                    if (input_window.is_in(x, y))
                    {
                        // Если пользователь кликнул по окну ввода
                        input_window.state = clicked;
                        SDL_StartTextInput();
                    }
                    else if (input_window.is_displayed)
                    {
                        // Если пользователь кликнул по другому участку
                        input_window.state = usual;
                        SDL_StopTextInput();
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                // Если пользователь отпустил кнопку мыши
                if (window_event.button.button == SDL_BUTTON_LEFT)
                {
                    // Если пользователь отпустил левую кнопку мыши
                    if (menu_start.state == clicked)
                    {
                        // Если пользователь отпустил кнопку мыши после того, как кликнул по пункту меню
                        menu_start.state = usual;
                    }
                    else if (menu_info.state == clicked)
                    {
                        // Если пользователь отпустил кнопку мыши после того, как кликнул по пункту меню
                        menu_info.state = usual;
                    }
                    else if (menu_exit.state == clicked)
                    {
                        // Если пользователь отпустил кнопку мыши после того, как кликнул по пункту меню
                        menu_exit.state = usual;
                    }
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                // Если перемещается мышка
                Sint32 x = window_event.motion.x;
                Sint32 y = window_event.motion.y;
                // Пункты меню
                if (menu_start.state == hovered && !menu_start.is_in(x, y))
                {
                    // Если пользователь вывел мушку с пункта меню
                    menu_start.state = usual;
                }
                else if (menu_start.state == !clicked && menu_start.is_in(x, y))
                {
                    // Если пользователь навел мышку на пункт меню
                    menu_start.state = hovered;
                }
                if (menu_info.state == hovered && !menu_info.is_in(x, y))
                {
                    // Если пользователь вывел мушку с пункта меню
                    menu_info.state = usual;
                }
                else if (menu_info.state == !clicked && menu_info.is_in(x, y))
                {
                    // Если пользователь навел мышку на пункт меню
                    menu_info.state = hovered;
                }
                if (menu_exit.state == hovered && !menu_exit.is_in(x, y))
                {
                    // Если пользователь вывел мушку с пункта меню
                    menu_exit.state = usual;
                }
                else if (menu_exit.state == !clicked && menu_exit.is_in(x, y))
                {
                    // Если пользователь навел мышку на пункт меню
                    menu_exit.state = hovered;
                }
                // Окно ввода
                if (input_window.is_in(x, y) && input_window.state != clicked)
                {
                    // Если пользователь навел мышку на окно ввода
                    input_window.state = hovered;
                }
                else if (!input_window.is_in(x, y) && input_window.state == hovered)
                {
                    // Если пользователь вывел мышку из окна ввода
                    input_window.state = usual;
                }
            }
            }
            draw(renderer);
        }
    }
    // Избавляемся от мусора
    SDL_DestroyTexture(score_texture);
    TTF_CloseFont(score_font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}