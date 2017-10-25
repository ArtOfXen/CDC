#include "Room.h"


Room::Room()
{
	_floorEntrance = false;
	_floorExit = false;
	_bossRoom = false;
	_leftmostRoom = false;
	_rightmostRoom = false;
	_unused = false;
}


Room::~Room()
{
}


void Room::setEntrance()
{
	_floorEntrance = true;
}

void Room::setExit()
{
	_floorExit = true;
}

void Room::setBossRoom()
{
	_bossRoom = true;
}

void Room::setFloor(int floor)
{
	_floor = floor;
}

void Room::setRoom(int room)
{
	_roomNum = room;
}

void Room::setLeftmost()
{
	_leftmostRoom = true;
}

void Room::setRightmost()
{
	_rightmostRoom = true;
}

void Room::setUnused()
{
	_unused = true;
}

void Room::setDoorLocations()
{
	// calculate which doors should be open in room

	_location = { false, false, false, false };

	if (_floorEntrance)
	{
		_location.entrance = true;
	}

	if (!_bossRoom && _floorExit)
	{
		_location.exit = true;
	}

	if (_leftmostRoom)
	{
		_location.leftmost = true;
	}

	if (_rightmostRoom)
	{
		_location.rightmost = true;
	}
}

int Room::getRoomNum()
{
	return _roomNum;
}

int Room::getFloorNum()
{
	return _floor;
}

roomLocation Room::location()
{
	return _location;
}

bool Room::bossRoom()
{
	return _bossRoom;
}

bool Room::isUnused()
{
	return _unused;
}
