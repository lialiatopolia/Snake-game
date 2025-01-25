/*
Файл с определением используемых в приложении констант.
*/

#include "const.h"

const char FILE_NAME[] = "snake_records.txt"; // имя файла, в который записываются рекорды игры
const char FONT[] = "Distroex.ttf"; // используемый шрифт
const char FONT2[] = "arial.ttf";
const int BLOCK_SIZE = 30; // размер одного блока/клетки по горизонтали и вертикали на игровом поле
const int HEIGHT = 600; // высота окна приложения
const int WIDTH = 600; // ширина окна приложения
const int FONT_MENU_SIZE = 35; // размер шрифта меню
const int FONT_TEXT_SIZE = 20; // размер шрифта обычного текста
const int FONT_TITLE_SIZE = 30; // размер шрифта заголовков
const int INPUT_WINDOW_HEIGHT = 80; // высота окна ввода
const int INPUT_WINDOW_WIDTH = 180; // ширина окна ввода
const int INPUT_WINDOW_Y = 300; // координата y верхнего левого угла окна ввода
const int MENU_HEIGHT = 60; // высота меню
const int MENU_ITEM_NUM = 3; // количество пунктов в меню: Старт, Справка, Выход
const int NAME_MAX_LENGTH = 18; // максимальная длина имени, которое может ввести пользователь
const int NX = 10; // максимальное количество клеток по горизонтали на игровом поле
const int NX_MIN = 5; // минимальное количество клеток по горизонтали на игровом поле
const int NY = 10; // максимальное количество клеток по вертикали на игровом поле
const int NY_MIN = 5; // минимальное количество клеток по вертикали на игровом поле
const int RECORDS_MAX_NUM = 5; // максимальное количество рекордов, выводимых на экран приложения
const SDL_Color BLACK = { 0, 0, 0, 0 }; // черный цвет
const SDL_Color DIM_GRAY = { 105, 105, 105, 0 }; // темно-серый цвет
const SDL_Color GREEN = { 0, 128, 0, 0 }; // зеленой цвет
const SDL_Color RED = { 255, 0, 0, 0 }; // красный цвет
const SDL_Color WHITE = { 255, 255, 250, 0 }; // белый цвет
const SDL_Color GAME_FIELD_COLOR = { 250, 220, 220, 255 }; // цвет игрового поля
const SDL_Color INPUT_CLICKED_COLOR = { 255, 182, 193, 255 }; // цвет окна ввода в кликнутом состоянии
const SDL_Color INPUT_HOVERED_COLOR = { 255, 153, 204, 255 }; // цвет окна ввода в наведенном состоянии
const SDL_Color INPUT_USUAL_COLOR = { 240, 230, 250, 255 }; // цвет окна ввода в обычном состоянии
const SDL_Color MENU_CLICKED_COLOR = { 200, 150, 180, 255 }; // цвет пункта меню в кликнутом состоянии
const SDL_Color MENU_HOVERED_COLOR = { 230, 190, 210, 255 }; // цвет пункта меню в наведенном состоянии
const SDL_Color MENU_USUAL_COLOR = { 245, 220, 235, 255 }; // цвет пункта меню в обычном состоянии
const SDL_Color SCREEN_COLOR = { 255, 223, 236, 255 }; // цвет экрана
const Uint32 GAME_TIME_INTERVAL = 500; // интервал между вызовами функции game_timer_callback во время игры