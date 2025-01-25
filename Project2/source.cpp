#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (!newTexture) {
        std::cerr << "Unable to load texture: " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
    }
    return newTexture;
}

void drawAirplane(SDL_Renderer* renderer, int x, int y, int scale) {
    // Fuselage
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect fuselage = { x, y, scale * 3, scale };
    SDL_RenderFillRect(renderer, &fuselage);

    // Wings
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderDrawLine(renderer, x + scale, y, x - scale, y + scale / 2);
    //SDL_RenderDrawLine(renderer, x + scale, y + scale, x - scale, y + scale / 2);
    SDL_RenderDrawLine(renderer, x + scale * 2, y, x + scale * 4, y + scale / 2);
    SDL_RenderDrawLine(renderer, x + scale * 2, y + scale, x + scale * 4, y + scale / 2);

    // Tail (from center of the fuselage)
    SDL_RenderDrawLine(renderer, x + scale * 1.5 + 10, y, x, y - scale / 2);
    SDL_RenderDrawLine(renderer, x + scale * 1.5 + 10, y + scale, x, y + scale + scale / 2);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Airplane Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Texture* background = loadTexture("background.png", renderer);
    if (!background) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    bool running = true;
    bool paused = false;
    bool landing = false;
    int airplaneX = 200;
    int airplaneY = 200; // Start at a fixed altitude
    int speed = 2;
    int scale = 20;
    float velocityX = 2.0f; // Horizontal velocity
    float velocityY = 0.0f; // Vertical velocity
    float gravity = 0.1f;  // Gravity for landing deceleration
    

    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_PLUS:
                case SDLK_EQUALS:
                    speed++;
                    break;
                case SDLK_MINUS:
                    if (speed > 1) speed--;
                    break;
                case SDLK_SPACE:
                    paused = !paused;
                    break;
                case SDLK_DOWN:
                    landing = true; // Trigger landing sequence
                    break;
                }
            }
        }
        if (!paused) {
        if (landing) {
            velocityY += gravity; // Simулируем гравитацию
            airplaneX += static_cast<int>(velocityX);
            airplaneY += static_cast<int>(velocityY);

            // Условие: самолет касается земли
            if (airplaneY + scale >= SCREEN_HEIGHT - 140) {
                airplaneY = SCREEN_HEIGHT - scale - 140; // Устанавливаем положение на земле
                velocityY = 0.0f; // Останавливаем вертикальное движение
                velocityX = 0.0f; // Останавливаем горизонтальное движение
                landing = false;  // Завершаем посадку
                paused = true;    // Пауза после остановки
            }
        }
    
    else {
        airplaneX += speed; // Continue horizontal flight
        if (airplaneX > SCREEN_WIDTH) {
            airplaneX = -scale * 3; // Reset position when off-screen
        }
    }
}

         
        

        SDL_RenderClear(renderer);

        // Render background
        SDL_RenderCopy(renderer, background, nullptr, nullptr);

        // Render airplane
        drawAirplane(renderer, airplaneX, airplaneY, scale);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
