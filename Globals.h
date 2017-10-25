#pragma once
#include <string>
#include <SDL\SDL.h>
#include <SDL_image\SDL_image.h>
#include <SDL_ttf\SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

enum MOVEMENT_DIRECTION
{
	NO_MOVEMENT,
	NEGATIVE_MOVEMENT,
	POSITIVE_MOVEMENT
};

enum FACING_DIRECTION
{
	UP_FACE,
	DOWN_FACE,
	LEFT_FACE,
	RIGHT_FACE
};

enum UNIT_TYPE
{
	NA,
	PLAYER_CHAR,
	ZOMBIE,
	SQUID,
	PROJECTILE
};

struct unit
{
	std::string spriteFilepath;
	int tilesetWidth;
	int tilesetHeight;
	int health;
	int moveDistance;
	UNIT_TYPE id;
};

struct highscore
{
	std::string name;
	int score;
	bool newHighscore;
};

struct roomLocation
{
	bool entrance;
	bool exit;
	bool leftmost;
	bool rightmost;
};

struct fontFilepaths
{
	std::string pixelFont;
	std::string miniFont;
};
