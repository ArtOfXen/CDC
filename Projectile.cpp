#include "Projectile.h"



Projectile::Projectile(int x, int y, int w, int h, unit unitStruct, int targetX, int targetY) : Character(x, y, w, h, unitStruct)
{
	// finds distance to target
	_targetX = targetX - x;
	_targetY = targetY - y;

	float totalTargetDistance = abs(_targetX) + abs(_targetY);

	// splits movement speed between x and y directions
	_velocityX = unitStruct.moveDistance * _targetX / totalTargetDistance;
	_velocityY = unitStruct.moveDistance * _targetY / totalTargetDistance;

	// a float value of the x and y positions. We need these so that we can add less than 1 to movement on each loop
	_floatX = x;
	_floatY = y;
}


Projectile::~Projectile()
{
}


void Projectile::moveProjectile(Uint32 thisLoopTime)
{
	
	if (thisLoopTime - _lastMoveTime >= _moveSpeed)
	{
		updateAnimationFrame(thisLoopTime);

		_floatX += _velocityX;
		_floatY += _velocityY;

		// round down float to int
		_x = _floatX;
		_y = _floatY;
		_collisionRect.x = _x;
		_collisionRect.y = _y;
	}
}