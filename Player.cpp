#include "Player.h"


Player::Player(int x, int y, int w, int h, unit unitStruct) : Character(x, y, w, h, unitStruct)

{
	_dir = DOWN_FACE; // face downwards by default

	_moveX = 0;
	_moveY = 0;

}


Player::~Player()
{
}


void Player::calcDirectionFacing()
{
	int mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	// changes (x, y) mouse coordinates to be relative to the player's current location
	int relativeX = mouseX - centreX();
	int relativeY = mouseY - centreY();

	// up and down
	if (abs(relativeY) > abs(relativeX))
	{
		if ((relativeX > 0 && relativeY <= 0) || (relativeX < 0 && relativeY < 0))
		{
			_dir = UP_FACE;
		}

		else
		{
			_dir = DOWN_FACE;
		}
	}

	// left and right
	else if (abs(relativeX) > abs(relativeY))
	{
		if ((relativeX < 0 && relativeY > 0) || (relativeX <= 0 && relativeY <= 0))
		{
			_dir = LEFT_FACE;
		}

		else
		{
			_dir = RIGHT_FACE;
		}
	}

	// in case all conditions are missed, default to facing down
	else
	{
		_dir = DOWN_FACE;
	}
}

bool Player::moveCharacter(Uint32 thisLoopTime)
{
	// check event queue for player movement

	MOVEMENT_DIRECTION x;
	MOVEMENT_DIRECTION y;

	SDL_Event e;

	std::vector<SDL_Event> otherEvents;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
			// add to movement if movement key pressed
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_w)
			{
				_moveY -= 1;
			}
			if (e.key.keysym.sym == SDLK_s)
			{
				_moveY += 1;
			}

			if (e.key.keysym.sym == SDLK_a)
			{
				_moveX -= 1;
			}
			if (e.key.keysym.sym == SDLK_d)
			{
				_moveX += 1;
			}
			break;

			// subtract from movement if movement key released
		case SDL_KEYUP:
			if ((e.key.keysym.sym == SDLK_w && _moveY < 0) || (e.key.keysym.sym == SDLK_s && _moveY > 0))
			{
				_moveY = 0;
			}

			if ((e.key.keysym.sym == SDLK_a && _moveX < 0) || e.key.keysym.sym == SDLK_d && _moveX > 0)
			{
				_moveX = 0;
			}
			break;

			// add other events to a vector
		default:
			otherEvents.push_back(e);
		}
	}
	
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	// check that none of the keys are pressed if movement is 0
	// if they are, then add to movement
	if (keyStates[SDL_SCANCODE_W] && !(keyStates[SDL_SCANCODE_S]) && _moveY == 0)
	{
		_moveY -= 1;
	}
	if (keyStates[SDL_SCANCODE_S] && !(keyStates[SDL_SCANCODE_W]) && _moveY == 0)
	{
		_moveY += 1;
	}
	if (keyStates[SDL_SCANCODE_A] && !(keyStates[SDL_SCANCODE_D]) && _moveX == 0)
	{
		_moveX -= 1;
	}
	if (keyStates[SDL_SCANCODE_D] && !(keyStates[SDL_SCANCODE_A]) && _moveX == 0)
	{
		_moveX += 1;
	}
	
	// returns POSITIVE (gte to 1), NEGATIVE (lte to -1), or NO DIRECTION
	x = calcMoveDirection(_moveX, 1, -1);
	y = calcMoveDirection(_moveY, 1, -1);

	if (x != NO_MOVEMENT || y != NO_MOVEMENT)
	{
		calcDirectionFacing();
	}

	// moves character
	bool playerMoved = Character::moveCharacter(x, y, NULL, thisLoopTime);

	// return unused events to event queue
	for (auto e : otherEvents)
	{
		SDL_PushEvent(&e);
	}

	return playerMoved;
}

FACING_DIRECTION Player::getDirectionFacing()
{
	return _dir;
}
