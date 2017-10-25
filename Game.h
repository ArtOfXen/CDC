#pragma once


#include "Globals.h"

class Game
{
public:

	SDL_Renderer* renderer;
	SDL_Window* gameWindow;
	SDL_Surface* gameSurface;

	Game();
	~Game();

	int screenWidth();
	int screenHeight();
	void generateRandSeed();

private:
	int _gameWindowWidth, _gameWindowHeight;
	bool _fullscreen;

};

