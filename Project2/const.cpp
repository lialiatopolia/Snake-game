/*
���� � ������������ ������������ � ���������� ��������.
*/

#include "const.h"

const char FILE_NAME[] = "snake_records.txt"; // ��� �����, � ������� ������������ ������� ����
const char FONT[] = "Distroex.ttf"; // ������������ �����
const char FONT2[] = "arial.ttf";
const int BLOCK_SIZE = 30; // ������ ������ �����/������ �� ����������� � ��������� �� ������� ����
const int HEIGHT = 600; // ������ ���� ����������
const int WIDTH = 600; // ������ ���� ����������
const int FONT_MENU_SIZE = 35; // ������ ������ ����
const int FONT_TEXT_SIZE = 20; // ������ ������ �������� ������
const int FONT_TITLE_SIZE = 30; // ������ ������ ����������
const int INPUT_WINDOW_HEIGHT = 80; // ������ ���� �����
const int INPUT_WINDOW_WIDTH = 180; // ������ ���� �����
const int INPUT_WINDOW_Y = 300; // ���������� y �������� ������ ���� ���� �����
const int MENU_HEIGHT = 60; // ������ ����
const int MENU_ITEM_NUM = 3; // ���������� ������� � ����: �����, �������, �����
const int NAME_MAX_LENGTH = 18; // ������������ ����� �����, ������� ����� ������ ������������
const int NX = 10; // ������������ ���������� ������ �� ����������� �� ������� ����
const int NX_MIN = 5; // ����������� ���������� ������ �� ����������� �� ������� ����
const int NY = 10; // ������������ ���������� ������ �� ��������� �� ������� ����
const int NY_MIN = 5; // ����������� ���������� ������ �� ��������� �� ������� ����
const int RECORDS_MAX_NUM = 5; // ������������ ���������� ��������, ��������� �� ����� ����������
const SDL_Color BLACK = { 0, 0, 0, 0 }; // ������ ����
const SDL_Color DIM_GRAY = { 105, 105, 105, 0 }; // �����-����� ����
const SDL_Color GREEN = { 0, 128, 0, 0 }; // ������� ����
const SDL_Color RED = { 255, 0, 0, 0 }; // ������� ����
const SDL_Color WHITE = { 255, 255, 250, 0 }; // ����� ����
const SDL_Color GAME_FIELD_COLOR = { 250, 220, 220, 255 }; // ���� �������� ����
const SDL_Color INPUT_CLICKED_COLOR = { 255, 182, 193, 255 }; // ���� ���� ����� � ��������� ���������
const SDL_Color INPUT_HOVERED_COLOR = { 255, 153, 204, 255 }; // ���� ���� ����� � ���������� ���������
const SDL_Color INPUT_USUAL_COLOR = { 240, 230, 250, 255 }; // ���� ���� ����� � ������� ���������
const SDL_Color MENU_CLICKED_COLOR = { 200, 150, 180, 255 }; // ���� ������ ���� � ��������� ���������
const SDL_Color MENU_HOVERED_COLOR = { 230, 190, 210, 255 }; // ���� ������ ���� � ���������� ���������
const SDL_Color MENU_USUAL_COLOR = { 245, 220, 235, 255 }; // ���� ������ ���� � ������� ���������
const SDL_Color SCREEN_COLOR = { 255, 223, 236, 255 }; // ���� ������
const Uint32 GAME_TIME_INTERVAL = 500; // �������� ����� �������� ������� game_timer_callback �� ����� ����