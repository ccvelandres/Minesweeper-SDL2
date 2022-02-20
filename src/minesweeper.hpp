#pragma once

#include <vector>
#include <cstdint>

#include "game.hpp"
#include <vectors.hpp>

class Minesweeper {
private:
    enum Tile {
        CLOSED,
        FLAG,
        MARK_CLOSED,
        MARK_OPEN,
        BOMB,
        BOMB_HIT,
        BOMB_MARK,
        OPEN,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        TILE_COUNT
    };

    std::vector<SDL_Texture*> tiles;

    Array2D<Tile> valueMap, textureMap;
    int size_x, size_y;
    int bombCount;

    SDL_Renderer *renderer;
    // Sets the render size of each tile
    const SDL_Rect srcRect = {0, 0, 32, 32}; 
    int lastPressX, lastPressY;

public:
    Minesweeper(SDL_Renderer *renderer, int x, int y);
    ~Minesweeper();

    void init(int bombs);
    void onClick(SDL_MouseButtonEvent &event);
    void update(int delta);
    void render();
};