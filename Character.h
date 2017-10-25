#pragma once
#include "Actor.h"

class Character : public Actor
{
public:

	Character(int x, int y, int w, int h, unit unitStruct);
	~Character();

	virtual bool moveCharacter(MOVEMENT_DIRECTION x, MOVEMENT_DIRECTION y, int customDistance = NULL, Uint32 thisLoopTime = NULL());
	int calcMoveDistance(MOVEMENT_DIRECTION dir, int unitMoveDistance);
	MOVEMENT_DIRECTION calcMoveDirection(int comparator, int comparator2, int comparator3);
	void takeDamage(int damage, Uint32 currentTime);
	void checkInvincibilityTimer(Uint32 currentTime);
	int health();
	UNIT_TYPE getID();
	void setLastAttackTime(Uint32 time);
	Uint32 getLastAttackTime();

protected:
	UNIT_TYPE _id;

	int _health;
	bool _invincible;
	float _lastDamagedTime, _invincibilityTimer;
	int _moveDistance;
	float _moveSpeed, _lastMoveTime;
	Uint32 _lastAttackTime;
	bool _canMoveLeft, _canMoveRight, _canMoveUp, _canMoveDown;


};

