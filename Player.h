#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(int x, int y, int w, int h, unit unitStruct);
	~Player();

	void calcDirectionFacing();
	FACING_DIRECTION getDirectionFacing();
	virtual bool moveCharacter(Uint32 thisLoopTime);

private:
	
	FACING_DIRECTION _dir;
	int _moveX;
	int _moveY;
};

