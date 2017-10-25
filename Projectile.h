#pragma once
#include "Character.h"

class Projectile : public Character
{
public:
	Projectile(int x, int y, int w, int h, unit unitStruct, int targetX, int targetY);
	~Projectile();

	void moveProjectile(Uint32 thisLoopTime);

private:
	int _targetX, _targetY;
	float _velocityX, _velocityY;
	float _floatX, _floatY;
};

