#pragma once
#include "Actor.h"
#include "Game.h"

class Text : public Actor
{
public:
	Text(std::string fontFilepath, SDL_Color textCol, int textSize, int x, int y, int w, int h, unit unitStruct);
	~Text();

	virtual void loadTexture(SDL_Renderer* renderer, std::string text);

private:
	TTF_Font* _font;
	SDL_Color _colour;
};

