#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

class TextureManager {
private:
    static std::string assetPath;
public:
    static void setAssetPath(const char* path) {
        assetPath = std::string(path);
    }

    static SDL_Texture* loadTexture(SDL_Renderer *renderer, const char *filename) {
        std::string texturePath = assetPath + "/textures/" + filename;
        SDL_Surface *surface = IMG_Load(texturePath.c_str());
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }
};