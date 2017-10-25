#include "Text.h"



Text::Text(std::string fontFilepath, SDL_Color textCol, int textSize, int x, int y, int w, int h, unit unitStruct) : Actor(x, y, w, h, unitStruct)
{
	_font = TTF_OpenFont(fontFilepath.c_str(), textSize);
	_colour = textCol;
}


Text::~Text()
{
}

void Text::loadTexture(SDL_Renderer* renderer, std::string text)
{
	// stops program from loading texture more than once
	if (_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(_font, text.c_str(), _colour);

	if (_width == NULL)
	{
		_width = textSurface->w;
	}

	if (_height == NULL)
	{
		_height = textSurface->h;
	}

	_texture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_FreeSurface(textSurface);

}

