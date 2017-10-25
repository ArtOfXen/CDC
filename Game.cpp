#include "Game.h"



Game::Game()
{
	// set up SDL

	SDL_INIT_EVERYTHING;
	IMG_INIT_PNG;
	TTF_Init();

	// create game window
	_gameWindowWidth = 1280;
	_gameWindowHeight = 720;

	gameWindow = SDL_CreateWindow("Dungeon Crawler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _gameWindowWidth, _gameWindowHeight, SDL_WINDOW_SHOWN);

	gameSurface = SDL_GetWindowSurface(gameWindow);

	renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}


Game::~Game()
{
}

int Game::screenWidth()
{
	return _gameWindowWidth;
}

int Game::screenHeight()
{
	return _gameWindowHeight;
}

void Game::generateRandSeed()
{
	std::srand(time(0));
}
