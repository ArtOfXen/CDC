#pragma once
#include "Character.h"
#include "Projectile.h"

class Room
{
public:
	Room();
	~Room();

	void setEntrance();
	void setExit();
	void setBossRoom();
	void setFloor(int floor);
	void setRoom(int room);
	void setLeftmost();
	void setRightmost();
	void setUnused();
	void setDoorLocations();

	int getRoomNum();
	int getFloorNum();
	bool bossRoom();
	bool isUnused();

	roomLocation location();

	std::vector <Character*> enemyList;
	std::vector <Projectile*> projectileList;

protected:
	roomLocation _location;
	int _floor;
	int _roomNum;

	bool _floorEntrance;
	bool _floorExit;
	bool _bossRoom;
	bool _leftmostRoom;
	bool _rightmostRoom;


	bool _unused;

};

