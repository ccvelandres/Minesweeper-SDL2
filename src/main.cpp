#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <iostream>
#include <game.hpp>
#include <textures.hpp>

Game *game = nullptr;

uint32_t computeFPS(uint32_t frameStart, uint32_t frameTime) {
	uint32_t delta = frameStart - frameTime;
	return 1;
}

int main(int argc, char** args)
{
	uint32_t frameStart = 0;
	uint32_t frameEnd = 0;
	uint32_t frameTime = 0;
	uint32_t frameDelta = 0;
	uint32_t targetFPS = 60;
	uint32_t targetDelta = 1000 / targetFPS;
	uint32_t fps;

	TextureManager::setAssetPath("assets");

	game = new Game();

	game->init("Minesweeper", 0, 0, 1024, 800, false);

	printf("TargetDelta: %10d\n", targetDelta);

	while(game->running()) {
		frameStart = SDL_GetTicks();
		frameDelta = frameStart - frameEnd;

		game->handleEvents();
		game->update(frameDelta);
		game->render();

		frameEnd = SDL_GetTicks();
		frameTime = frameEnd - frameStart;
		if (targetDelta > frameTime) {
			SDL_Delay(targetDelta - frameTime);
		}
		fps = 1000 / (SDL_GetTicks() - frameStart);
	}

	game->clean();

    return 0;
}