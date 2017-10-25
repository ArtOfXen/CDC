#pragma once

#include <SDL\SDL.h>
#include <SDL_image\SDL_image.h>
#include <ctime>
#include<iostream>
#include<vector>
#include "Globals.h"


class Actor
{
	// For anything the player can interact with and is drawn onto the screen
public:

	static const Uint32 animationLoopTime = 250;

	Actor(int x, int y, int w, int h, unit unitStruct);
	~Actor();

	virtual void loadTexture(SDL_Renderer* renderer);
	void unloadTexture();
	void render(SDL_Renderer* renderer, int w = NULL, int h = NULL, SDL_Rect* clipRect = NULL, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	
	SDL_Rect getCollisionRect();
	std::vector <SDL_Rect> getMoreCollisionRects();
	SDL_Texture* getTexture();
	void setTextureColour(int r = 255, int g = 255, int b = 255);

	void setLocation(int x, int y);
	int updateAnimationFrame(Uint32 time, bool resetAnimation = false);

	int setCentreX(int screenCentreX);
	int setCentreY(int screenCentreY);

	int getAnimationFrame();
	int tilesetWidth();
	int tilesetHeight();

	int actorWidth();
	int actorHeight();
	int centreX();
	int centreY();

protected:
	SDL_Texture* _texture;

	Uint32 _lastAnimationChange;
	std::string _filepath;
	int _tilesetWidth, _tilesetHeight, _animationFrame;
	int _width, _height;
	float _widthStretchFactor, _heightStretchFactor;
	int _x, _y;
	SDL_Rect _collisionRect;
};

