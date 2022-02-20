#include <game.hpp>

#include <iostream>
#include <exception>

Game::Game() {

}

Game::~Game() {

}

void Game::init(const char *title, int x, int y, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("ERROR: Could not initialize SDL");
    }
    std::cout << "SDL Initialized!" << std::endl;

    window = SDL_CreateWindow(title, x, y, width, height, flags);
    if (!window) {
        throw std::runtime_error("ERROR: Could not create window");
    }
    std::cout << "Window created!" << std::endl;

    renderer = SDL_CreateRenderer(window, -1 , 0);
    if (!renderer) {
        throw std::runtime_error("ERROR: Could not create renderer");
    }
    std::cout << "Renderer created!" << std::endl;

    isRunning = true;

    mines = new Minesweeper(renderer, 16,16);
    mines->init(30);
}

void Game::handleEvents() {
    SDL_Event event;
    while( SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                // printf("MouseButtonEvent: %4d,%4d\n", event.button.x, event.button.y);
                mines->onClick(event.button);
                break;
            default:
                break;
        }
    }
}

void Game::update(uint32_t delta) {
    mines->update(delta);
}

void Game::render() {
    // probably better if we render only in texture map changes
    SDL_RenderClear(renderer);
    mines->render();
    SDL_RenderPresent(renderer);
    SDL_Delay(5);
}

void Game::clean() {
    delete mines;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}   