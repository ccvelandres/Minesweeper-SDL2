#pragma once

#include <SDL2/SDL.h>

#include <minesweeper.hpp>

class Game {
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isRunning;
    int targetFPS;

    class Minesweeper *mines;
public:
    Game();
    ~Game();
 
    void init(const char *title, int x, int y, int width, int height, bool fullscreen);
    void handleEvents();
    void update(uint32_t delta);
    void render();
    void clean(); 

    int getTargetFPS() { return targetFPS; }
    void setTargetFPS(int fps) { targetFPS = fps; };
    bool running() { return isRunning; }
};