#include "Character.h"
#include <iostream>


Character::Character(int x, int y, int w, int h, unit unitStruct) : Actor(x, y, w, h, unitStruct)
{
	_lastMoveTime = SDL_GetTicks();
	_lastAttackTime = SDL_GetTicks();
	_lastDamagedTime = SDL_GetTicks();

	_moveSpeed = 1000 / 60; // move no more than 60 times per second

	_invincible = false;
	_invincibilityTimer = 750;

	_health = unitStruct.health;
	_moveDistance = unitStruct.moveDistance;
	_id = unitStruct.id;
}


Character::~Character()
{
}


 bool Character::moveCharacter(MOVEMENT_DIRECTION x, MOVEMENT_DIRECTION y, int customDistance, Uint32 thisLoopTime)
{
	bool characterMoved = false;
	if (thisLoopTime - _lastMoveTime >= _moveSpeed || thisLoopTime == NULL)
	{
		 int moveX, moveY, moveDist;

		 (customDistance != NULL) ? moveDist = customDistance : moveDist = _moveDistance;

		 moveX = calcMoveDistance(x, moveDist);
		 moveY = calcMoveDistance(y, moveDist);

		// if the player is moving diagonally, we lower the move distance a bit
		if (x != NO_MOVEMENT && y != NO_MOVEMENT)
		{
			moveX = moveX * 3/4;
			moveY = moveY * 3/4;
		}

		// update the object's movement and positional data
		_lastMoveTime = thisLoopTime;
		_x += moveX;
		_y += moveY;
		_collisionRect.x = _x;
		_collisionRect.y = _y;

		// check if movement animation should be updated
		if (x != NO_MOVEMENT || y != NO_MOVEMENT)
		{
			characterMoved = true;
			updateAnimationFrame(thisLoopTime);
		}
	}

	return characterMoved;
}

 int Character::calcMoveDistance(MOVEMENT_DIRECTION dir, int unitMoveDistance)
 {
	 // converts Positive or Negative into integers

	 int d;

	 if (dir == NO_MOVEMENT)
	 {
		 d = 0;
	 }
	 else if (dir == POSITIVE_MOVEMENT)
	 {
		 d = unitMoveDistance;
	 }
	 else if (dir == NEGATIVE_MOVEMENT)
	 {
		 d = -unitMoveDistance;
	 }

	 return d;

 }

 MOVEMENT_DIRECTION Character::calcMoveDirection(int comparator, int comparator2, int comparator3)
 {
	 // non-player controlled characters always move towards or away from something

	 MOVEMENT_DIRECTION dir;

	 if (comparator3 == NULL)
	 {
		 comparator3 = comparator2;
	 }

	 if (comparator >= comparator2)
	 {
		 dir = POSITIVE_MOVEMENT;
	 }
	 else if (comparator <= comparator3)
	 {
		 dir = NEGATIVE_MOVEMENT;
	 }
	 else
	 {
		 dir = NO_MOVEMENT;
	 }

	 return dir;
 }

 void Character::takeDamage(int damage, Uint32 currentTime)
 {
	 if (!_invincible) 
	 {
		 _health -= damage;
		 _invincible = true;
		 _lastDamagedTime = currentTime;
		 setTextureColour(255, 0, 0); // set character colour to red
	 }
 }

 void Character::checkInvincibilityTimer(Uint32 currentTime)
 {
	 if (_invincible && currentTime - _lastDamagedTime >= _invincibilityTimer)
	 {
		 _invincible = false;
		 setTextureColour(); // set character colour to normal
	 }
 }

 int Character::health()
 {
	 return _health;
 }

 void Character::setLastAttackTime(Uint32 time)
 {
	 _lastAttackTime = time;
 }

 Uint32 Character::getLastAttackTime()
 {
	 return _lastAttackTime;
 }

 UNIT_TYPE Character::getID()
 {
	 return _id;
 }
