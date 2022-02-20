#include "minesweeper.hpp"

#include <vector>
#include <array>
#include <iostream>
#include <random>
#include <stack>
#include <thread>
#include <random>

#include <textures.hpp>

Minesweeper::Minesweeper(SDL_Renderer *renderer, int x, int y) : valueMap(x, y), textureMap(x, y)
{
    size_x = x;
    size_y = y;
    this->renderer = renderer;

    if (tiles.empty())
    {
        tiles.reserve(TILE_COUNT);
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_00.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_01.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_02.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_03.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_04.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_05.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_06.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_07.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_08.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_09.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_10.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_11.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_12.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_13.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_14.png"));
        tiles.emplace_back(TextureManager::loadTexture(renderer, "minesweeper_15.png"));
    }
}

Minesweeper::~Minesweeper()
{
    for (auto &tile : tiles)
    {
        SDL_DestroyTexture(tile);
    }
}

void Minesweeper::init(int bombs)
{
    int c = 0;
    bombCount = bombs;

    if (bombs > size_x * size_y)
        throw std::invalid_argument("bombCount larger than total tiles");

    while (c < bombs)
    {
        std::minstd_rand0 random(std::chrono::system_clock::now().time_since_epoch().count());
        int j = random() % size_x;
        int k = random() % size_y;
        if (valueMap.at(j, k) != BOMB)
        {
            valueMap.at(j, k) = BOMB;
            c++;
        }
    }

    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
        {
            int bombNeighbors = 0;
            if (valueMap.at(x, y) == BOMB)
                continue;
            for (int j = x - 1; j <= x + 1; j++)
            {
                for (int k = y - 1; k <= y + 1; k++)
                {
                    if (j >= 0 && k >= 0 && j < size_x && k < size_y)
                    {
                        if (valueMap.at(j, k) == BOMB)
                            bombNeighbors++;
                    }
                }
            }
            valueMap.at(x, y) = (Tile)(OPEN + bombNeighbors);
        }
    }
}

void Minesweeper::update(int delta)
{
}

void Minesweeper::render()
{
    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
        {
            SDL_Rect dstRect = {x * srcRect.w, y * srcRect.h, srcRect.w, srcRect.h};
            SDL_RenderCopy(renderer, tiles[textureMap.at(x, y)], NULL, &dstRect);
        }
    }

    // texturemap
    for (int x = 0; x < size_x; x++)
    {
        for (int y = 0; y < size_y; y++)
        {
            SDL_Rect dstRect = {(size_x * srcRect.w) + x * srcRect.w, y * srcRect.h, srcRect.w, srcRect.h};
            SDL_RenderCopy(renderer, tiles[valueMap.at(x, y)], NULL, &dstRect);
        }
    }
}

void Minesweeper::onClick(SDL_MouseButtonEvent &event)
{
    // I know... this function is obscene...
    int tilex = event.x / srcRect.w;
    int tiley = event.y / srcRect.h;

    if (tilex >= 0 && tiley >= 0 && tilex < size_x && tiley < size_y)
    {
        printf("Button: %d State: %d Clicks: %d @ %d,%d\n",
               event.button, event.state, event.clicks, tilex, tiley);
        if (event.state == SDL_PRESSED && event.button == SDL_BUTTON_LEFT)
        {
            lastPressX = tilex;
            lastPressY = tiley;
            if (textureMap.at(tilex, tiley) != CLOSED)
                return;
            textureMap.at(tilex, tiley) = OPEN;
            printf("PressDown @ %d,%d\n", lastPressX, lastPressY);
        }
        else if (event.state == SDL_RELEASED)
        {
            printf("Release @ %d,%d\n", tilex, tiley);
            if (event.button == SDL_BUTTON_LEFT)
            {
                if (tilex != lastPressX || tiley != lastPressY)
                {
                    textureMap.at(lastPressX, lastPressY) = CLOSED;
                    return;
                }
                // do nothing if tile is already flipped
                // BOMB: flip all tiles and all bombs to bomb_hit
                if (valueMap.at(tilex, tiley) == BOMB)
                {
                    for (int i = 0; i < valueMap.vector().size(); i++)
                        if (valueMap.vector()[i] == BOMB)
                            textureMap.vector()[i] = BOMB_HIT;
                        else
                            textureMap.vector()[i] = valueMap.vector()[i];
                }
                else if (valueMap.at(tilex, tiley) == OPEN)
                {
                    // flood-fill algorithm with flip on edges

                    std::thread *th = new std::thread([tilex, tiley, this](){
                    int iter = 0;
                    const std::array<int, 4> dx = {1, 0, -1, 0};
                    const std::array<int, 4> dy = {0, 1, 0, -1};

                    std::stack<Point> stack;
                    stack.push({tilex, tiley});

                    while (!stack.empty())
                    {
                        Point p = stack.top();
                        stack.pop();
                        textureMap.at(p.x, p.y) = OPEN;

                        int sx = 0;
                        int sy = 0;
                        for (int i = 0; i < dx.size(); i++)
                        {
                            int x = p.x + dx[i];
                            int y = p.y + dy[i];
                            if (x >= 0 && y >= 0 && x < size_x && y < size_y)
                            {
                                if (valueMap.at(x, y) == OPEN)
                                {
                                    if (textureMap.at(x, y) == CLOSED)
                                        stack.push({x, y});
                                    textureMap.at(x, y) = OPEN;
                                }
                                else
                                {
                                    textureMap.at(x, y) = valueMap.at(x, y);
                                    sx += std::abs(dx[i]);
                                    sy += std::abs(dy[i]);
                                }
                            }
                        }

                        printf("floodfill %2d: %2d,%2d sx:%d sy:%d s+: %d ", iter++, p.x, p.y, sx, sy, sx+sy);

                        if (sx + sy == 2) {
                            int dX = valueMap.at(p.x+1,p.y) != OPEN ? 1 : -1;
                            int dY = valueMap.at(p.x,p.y+1) != OPEN ? 1 : -1;
                            printf("+2: %d,%d ", dX, dY);
                            textureMap.at(p.x+dX, p.y+dY) = valueMap.at(p.x+dX, p.y+dY); 
                            if (valueMap.at(p.x+dX, p.y+dY) == OPEN)
                                stack.push({p.x+dX, p.y+dY});
                        }
                        else if (sx + sy == 3) {
                            const std::array<int, 2> offset = {1,-1};
                            if (sx == 2) {
                                int dY = valueMap.at(p.x, p.y+1) ? 1 : 0;
                                printf("+3: dY=%d ", dY);
                                for (auto& it : offset) {
                                    int dX1 = p.x + it;
                                    int dY1 = p.x + dY;
                                    textureMap.at(dX1, dY1) = valueMap.at(dX1, dY1);
                                    if (valueMap.at(dX1, dY1) == OPEN)
                                        stack.push({dX1, dY1});
                                }
                            }
                            else if (sy == 2) {
                                int dX = valueMap.at(p.x+1, p.y) ? 1 : 0;
                                printf("+3: dX=%d ", dX);
                                for (auto& it : offset) {
                                    int dX1 = p.x + dX;
                                    int dY1 = p.x + it;
                                    textureMap.at(dX1, dY1) = valueMap.at(dX1, dY1);
                                    if (valueMap.at(dX1, dY1) == OPEN)
                                        stack.push({dX1, dY1});
                                }
                            }
                        }
                        printf("\n");
                        // if (sx + sy == 2) {
                        //     // try L
                        // }
                        // else if (sx )

                        SDL_Delay(1000);
                    }
                    });
                }
                else
                {
                    // flip tile
                    textureMap.at(tilex, tiley) = valueMap.at(tilex, tiley);
                }
            }
            else if (event.button == SDL_BUTTON_RIGHT)
            {
                switch (textureMap.at(tilex, tiley))
                {
                case CLOSED:
                    textureMap.at(tilex, tiley) = FLAG;
                    break;
                case FLAG:
                    textureMap.at(tilex, tiley) = CLOSED;
                    break;
                default:
                    break;
                }
            }
            else if (event.button == SDL_BUTTON_MIDDLE)
            {
                printf("Middle @ %d, %d\n", tilex, tiley);
                // check neighbors if matches b
                int bombNeighbors = 0;
                int flagNeighbors = 0;
                for (int j = tilex - 1; j <= tilex + 1; j++)
                {
                    for (int k = tiley - 1; k <= tiley + 1; k++)
                    {
                        if (j >= 0 && k >= 0 && j < size_x && k < size_y)
                        {
                            if (valueMap.at(j, k) == BOMB) {
                                if (textureMap.at(j, k) == FLAG)
                                {
                                    bombNeighbors++;
                                    flagNeighbors++;
                                }
                                else
                                {
                                    return;
                                }
                            }
                        }
                    }
                }

                if (bombNeighbors && bombNeighbors == flagNeighbors)
                {
                    const std::array<int, 8> dx = {-1, -1, -1, 0, 0, 1, 1, 1};
                    const std::array<int, 8> dy = {-1, 0, 1, -1, 1, -1, 0, 1};

                    for (int i = 0; i < dx.size(); i++)
                    {
                        int x = tilex + dx[i];
                        int y = tiley + dy[i];
                        if (x >= 0 && y >= 0 && x < size_x && y < size_y)
                            if (textureMap.at(x, y) == CLOSED)
                                textureMap.at(x, y) = valueMap.at(x, y);
                    }
                }
            }
        }
    }
    else
    {
        printf("OutOfBounds @ %d,%d", tilex, tiley);
    }
    printf("\n");
}