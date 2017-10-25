#include "Actor.h"



Actor::Actor(int x, int y, int w, int h, unit unitStruct)
{

	_width = w; _height = h; _texture = NULL;
	_x = x; _y = y;

	_filepath = unitStruct.spriteFilepath;
	_tilesetWidth = unitStruct.tilesetWidth;
	_tilesetHeight = unitStruct.tilesetHeight;

	_collisionRect = { _x, _y, _width, _height };
	_lastAnimationChange = SDL_GetTicks();
}


Actor::~Actor()
{
}

void Actor::loadTexture(SDL_Renderer* renderer)
{
	// stops us loading texture more than once
	if (_texture != NULL) {
		SDL_DestroyTexture(_texture);
	}
	
	SDL_Surface* loadFile = IMG_Load(_filepath.c_str());

	SDL_SetColorKey(loadFile, 1, SDL_MapRGB(loadFile->format, 255, 255, 255));

	if (_width == NULL)
	{
		_width = loadFile->w;
		_collisionRect.w = _width;
	}

	if (_height == NULL)
	{
		_height = loadFile->h;
		_collisionRect.h = _height;
	}

	_texture = SDL_CreateTextureFromSurface(renderer, loadFile);

	SDL_FreeSurface(loadFile); // don't need surface anymore so delete

}

void Actor::unloadTexture()
{
	if (_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
	}
}


void Actor::render(SDL_Renderer* renderer, int w, int h, SDL_Rect* clipRect, double angle, SDL_RendererFlip flip)
{
	if (w == NULL)
	{
		w = actorWidth();
		
	}
	if (h == NULL)
	{
		h = actorHeight();
	}

	if (_collisionRect.w != w)
	{
		_collisionRect.w = w;
	}
	if (_collisionRect.h != h)
	{
		_collisionRect.h = h;
	}


	SDL_Rect renderRect = { _x, _y, w, h };


	int error = SDL_RenderCopyEx(renderer, _texture, clipRect, &renderRect, angle, NULL, flip);

	// checks if render correctly
	if (error != 0)
	{
		printf(SDL_GetError());
	}
}


int Actor::actorWidth() 
{
	return _width / _tilesetWidth;
}


int Actor::actorHeight()
{
	return _height / _tilesetHeight;
}


SDL_Rect Actor::getCollisionRect()
{
	return _collisionRect;
}

std::vector <SDL_Rect> Actor::getMoreCollisionRects()
{
	// create rect for each side of actor
	SDL_Rect top = {_x + _collisionRect.w * 1 / 5, _y, _collisionRect.w * 3 / 5, _collisionRect.h * 1 / 5 };
	SDL_Rect bottom = { _x + _collisionRect.w * 1 / 5, _y + _collisionRect.h * 4 / 5, _collisionRect.w * 3 / 5, _collisionRect.h * 1 / 5 };
	SDL_Rect left = { _x, _y + _collisionRect.h * 1 / 5, _collisionRect.w * 1 / 5, _collisionRect.h * 3 / 5 };
	SDL_Rect right = { _x + _collisionRect.w * 4 / 5 , _y + _collisionRect.h * 1 / 5, _collisionRect.w * 1 / 5, _collisionRect.h * 3 / 5 };

	std::vector <SDL_Rect> sides = { top, bottom, left, right };
	return sides;
}

SDL_Texture* Actor::getTexture()
{
	return _texture;
}

void Actor::setTextureColour(int r, int g, int b)
{
	SDL_SetTextureColorMod(_texture, r, g, b);
}

void Actor::setLocation(int x, int y)
{
	_x = x;
	_y = y;
	_collisionRect.x = x;
	_collisionRect.y = y;
}

int Actor::updateAnimationFrame(Uint32 time, bool resetAnimation)
{
	if (time - _lastAnimationChange >= animationLoopTime)
	{
		if (resetAnimation == true)
		{
			_animationFrame = 0;
		}

		else
		{
			_animationFrame++;

			if (_animationFrame >= _tilesetWidth)
			{
				_animationFrame = 0;
			}
		}

		_lastAnimationChange = time;
	}

	return _animationFrame;
}

int Actor::getAnimationFrame()
{
	return _animationFrame;
}

int Actor::setCentreX(int screenCentreX)
{
	int x = screenCentreX - actorWidth() / 2;
	return x;
}

int Actor::setCentreY(int screenCentreY)
{
	int y = screenCentreY - actorHeight() / 2;
	return y;
}

int Actor::tilesetWidth()
{
	return _tilesetWidth;
}

int Actor::tilesetHeight()
{
	return _tilesetHeight;
}


int Actor::centreX()
{
	int cx = _x + actorWidth() / 2;
	return cx;
}

int Actor::centreY()
{
	int cy = _y + actorHeight() / 2;
	return cy;
}