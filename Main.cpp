#include "Main.h"

// game object stores SDL info like game window and renderer
Game game;
bool exitGame;

// declare time variables
Uint32 thisLoopTime = SDL_GetTicks();
Uint32 playerAttackStart;
Uint32 playerAttackLength = 500;

// these data structs are used to determine specific variables within classes
// struct parameters: spritesheet filepath, spritesheet width, spritesheet height, health, movespeed, unit type

	// actors
unit backgroundData = { "textures/background.png",	1, 1, 255,	0, NA };
unit tbWallFillData = { "textures/topWall.png",		1, 1, 255,	0, NA };
unit lrWallFillData = { "textures/sideWall.png",	1, 1, 255,	0, NA };
unit treasureData = { "textures/chest.png",			1, 1, 255,	0, NA };

unit swordData = { "textures/sword.png",			4, 1, 255,	0, NA };
unit squidProjectileData{ "textures/goo.png",		2, 1, 1,	1, PROJECTILE };

unit uiScoreData = { "textures/gem.png",			2, 1, 255,	0, NA };
unit uiLifeData = { "textures/life.png", 			2, 1, 255,	0, NA };

unit textData =		 { "",							1, 1, 255,	0, NA };

	// characters
unit zombieData = { "textures/zombie.png",			2, 2, 1,	2, ZOMBIE };
unit squidData = { "textures/squid.png", 			2, 2, 1,	0, SQUID};
unit bossData = { "textures/boss.png",				1, 1, 2,	0, BOSS};

unit playerData = { "textures/knightAnimation.png", 4, 4, 3,	6, PLAYER_CHAR };

// other filepaths

	// text files
std::string difficultyFileName = "difficulty.txt";
std::string highscoresFileName = "highscores.txt";

	// fonts
fontFilepaths fonts = { "textures/fontPixel.ttf", "textures/fontMini.ttf" };


// Colours
SDL_Color white = { 255, 255, 255 };
SDL_Color gold = { 255, 223, 0 };

// Actors
Actor treasure(game.screenWidth() / 2, game.screenHeight() / 2, NULL, NULL, treasureData);
Actor background( 0, 0, game.screenWidth(), game.screenHeight(), backgroundData);
Actor swordAttack(NULL, NULL, NULL, NULL, swordData);

bool playerAttacking = false;

// the size of the background image in tiles
const int BACKGROUND_TILES_X = 25;
const int BACKGROUND_TILES_Y = 15;

// split screen into tiles of set size
const int tileWidth = background.actorWidth() / BACKGROUND_TILES_X;
const int tileHeight = background.actorHeight() / BACKGROUND_TILES_Y;

int tbDoorWidth;	// top and bottom door size
int lrDoorHeight;	// left and right door size

// images that are rendered over doors that are unused
Actor tbWallFill(0, 0, tileWidth * 4, tileHeight * 2, tbWallFillData);
Actor lrWallFill(0, 0,tileWidth * 2, tileHeight * 4, lrWallFillData);

// used for enemy spawn locations
SDL_Rect playableArea = { tileWidth * 3, tileHeight * 3, background.actorWidth() - tileWidth * 6, background.actorHeight() - tileHeight * 6 };

// vectors
std::vector<std::vector <Room>> roomList;
std::vector <SDL_Rect> doors;
std::vector <SDL_Rect> wallCollisionRects;
std::vector <std::string> levelDifficultyChanges;

// iterators
std::vector <Character*>::iterator enemyIterator;
std::vector <Projectile*> ::iterator projectileIterator;


// define score / level here so the variable can be used in multiple functions
int score;
int level;

int main(int argc, char* args[])
{
	startMenu();
	return 0;
}

void startMenu()
{
	// create class objects
		// different text object for when option is hovered over so that colour changes
	Text unselectedOption(fonts.pixelFont, white, 72, NULL, NULL, NULL, NULL, textData);
	Text selectedOption(fonts.pixelFont, gold, 72, NULL, NULL, NULL, NULL, textData);

	// event variables
	SDL_Event e;
	int mouseX, mouseY;
	SDL_Rect mousePointer;

	// create an option for each button on the menu
	struct option {
		std::string name;
		SDL_Rect rect;
		bool clicked;
	};

	option Start;
	Start.name = "Start";

	option Highscores;
	Highscores.name = "Highscores";

	option Quit;
	Quit.name = "Quit";

	std::vector<option> options = { Start, Highscores, Quit };
	int numOfOptions = options.size();

	for (int i = 0; i < options.size(); i++)
	{
		int width = options[i].name.length() * tileWidth * 3/4 ;
		options[i].rect = { background.actorWidth() / 2 - width / 2, background.actorHeight() * (i + 1) / (numOfOptions + 1) - tileHeight * 3/4, width, tileHeight * 3/2};
		options[i].clicked = false;
	}

	//
	// menu draw loop
	//
	exitGame = false;
	while (!exitGame)
	{
		SDL_SetRenderDrawColor(game.renderer, 107, 83, 91, 255);
		SDL_RenderClear(game.renderer);

		while (SDL_PollEvent(&e) != 0)
		{

			switch (e.type)
			{
			case SDL_QUIT:
				return;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouseX, &mouseY);
				// mouse rect used to check collision when clicked
				mousePointer = { mouseX, mouseY, 1, 1 };
				break;

			case SDL_MOUSEBUTTONDOWN:
				// check if option has been clicked on
				for (int i = 0; i < options.size(); i++)
				{
					if (SDL_HasIntersection(&options[i].rect, &mousePointer))
					{
						options[i].clicked = true;
					}
				}
				break;
			}
		}

		for (int i = 0; i < options.size(); i++)
		{
			Text* text;

			if (options[i].clicked)
			{
				switch (i)
				{
					// start game
				case 0:
					exitGame = mainGame();
					break;

				case 1:
					// show highscores
					exitGame = updateHighscores(NULL);
					break;

				case 2:
					// exit game
					return;
				}

				options[i].clicked = false;
			}

			// set correct Text object
			if (SDL_HasIntersection(&options[i].rect, &mousePointer))
			{
				text = &selectedOption;
			}
			else
			{
				text = &unselectedOption;
			}

			// draw to screen
			text->loadTexture(game.renderer, options[i].name);
			text->setLocation(options[i].rect.x, options[i].rect.y);
			text->render(game.renderer, options[i].rect.w, options[i].rect.h);
		}

		// check that game has no quit before updating, otherwise the menu is drawn no matter when the player quits
		if (!exitGame)
		{
			SDL_RenderPresent(game.renderer);
		}
	}
}

bool mainGame()
{
	// set game difficulty variables from file
	const int DIFFICULTY_FILE_VARIABLES = 5;
	int difficultyVariables[DIFFICULTY_FILE_VARIABLES];
	readDifficultyFile(difficultyVariables, DIFFICULTY_FILE_VARIABLES);
	
	int floorsPerLevel = difficultyVariables[0];
	if (floorsPerLevel < 2) { floorsPerLevel = 2; }

	int roomsPerFloor = difficultyVariables[1];
	if (roomsPerFloor < 2) { roomsPerFloor = 2; }

	int maxEnemiesPerRoom = difficultyVariables[2];
	if (maxEnemiesPerRoom < 1) { maxEnemiesPerRoom = 1; }

	playerData.moveDistance = difficultyVariables[3];
	if (playerData.moveDistance <= 0) { playerData.moveDistance = 1; }

	int enemyBonusMoveDistance = difficultyVariables[4];
	if (enemyBonusMoveDistance < 0) { enemyBonusMoveDistance = 0; }
	zombieData.moveDistance += enemyBonusMoveDistance;
	squidProjectileData.moveDistance += enemyBonusMoveDistance;

	

	level = 1;
	score = 0;
	Player player(background.centreX(), background.centreY(), NULL, NULL, playerData);
	Actor uiScore(tileWidth * (BACKGROUND_TILES_X / 4), tileHeight / 4, NULL, NULL, uiScoreData);
	Actor uiLife(tileWidth * (BACKGROUND_TILES_X * 3 / 4), tileHeight / 4, NULL, NULL, uiLifeData);

	// load object textures
	player.loadTexture(game.renderer);
	swordAttack.loadTexture(game.renderer);
	treasure.loadTexture(game.renderer);
	background.loadTexture(game.renderer);
	tbWallFill.loadTexture(game.renderer);
	lrWallFill.loadTexture(game.renderer);

	// set door size based on player size
	tbDoorWidth = player.actorWidth() * 2;	// top and bottom door size
	lrDoorHeight = player.actorHeight() * 2;	// left and right door size

	// define UI objects
	std::vector<Actor> uiElements;

	uiScore.loadTexture(game.renderer);
	uiElements.push_back(uiScore);

	uiLife.loadTexture(game.renderer);
	uiElements.push_back(uiLife);

	Text uiScoreText(fonts.miniFont, white, 36, uiScore.getCollisionRect().x + tileWidth, 0, NULL, NULL, textData);

	Text uiLifeText(fonts.miniFont, white, 36, uiLife.getCollisionRect().x - tileWidth, 0, NULL, NULL, textData);

	// creates the collision rects that will always stay the same throughout the game
	createConstCollisionRects(&doors, &wallCollisionRects, tbDoorWidth, lrDoorHeight);
	

	//
	// Game loop
	//
	bool gameOver = false;
	while (!gameOver)
	{

		//generate Rooms for this level
		int currentFloorNum = 0;
		int currentRoomNum;

		std::vector<std::vector <Room>> roomList = generateLevelRooms(roomsPerFloor, floorsPerLevel, maxEnemiesPerRoom);

		// finds the starting room (entrance room on the 0th floor)
		for (int i = 0; i < roomsPerFloor; i++)
		{
			if (roomList[currentFloorNum][i].location().entrance)
			{
				currentRoomNum = i;
				break;
			}
		}

		// create pointer to room the player is in
		Room *currentRoom;
		currentRoom = changeRoom(roomList, currentFloorNum, currentRoomNum, tbDoorWidth, lrDoorHeight);


		bool levelInProgress = true; // true until level complete or player dies

		SDL_Event e;


		prelevelScreen(level, score, uiScore, player.health(), uiLife, levelDifficultyChanges);


		// difficulty vector no longer needed so clear it
		levelDifficultyChanges.clear();
		
		//
		// Level loop
		//

		while (levelInProgress) {

			//reset variables and background, which will be redefined / redrawn during the loop
			SDL_SetRenderDrawColor(game.renderer, 107, 83, 91, 255);
			SDL_RenderClear(game.renderer);

			thisLoopTime = SDL_GetTicks();

			// end game if player dead
			if (player.health() <= 0)
			{
				gameOver = true;
				levelInProgress = false;
			}

			//player move
			if (!player.moveCharacter(thisLoopTime) && player.getAnimationFrame() != 0) {
				//reset animation if not moving
				player.updateAnimationFrame(thisLoopTime, true);
			}

			// event loop
			while (SDL_PollEvent(&e) != 0)
			{

				switch (e.type)
				{
				case SDL_QUIT:
					return true;

				case SDL_MOUSEMOTION:
					player.calcDirectionFacing();
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (playerAttacking == false)
					{
						playerAttacking = true;
						playerAttackStart = SDL_GetTicks();
					}
					break;
				}
			}

			// check player collisions
			checkWallCollisions(wallCollisionRects, player);
			currentRoom = checkDoorCollisions(&player, &doors, currentRoom, roomList, tbDoorWidth, lrDoorHeight);

			// render background
			background.render(game.renderer);
			coverUnusedDoors(currentRoom);


			// create treasure if in last room of bottom floor
			if (currentRoom->getFloorNum() == floorsPerLevel - 1 && currentRoom->bossRoom())
			{
				if (currentRoom->enemyList.size() <= 0)
				{
					treasure.setCentreX(background.centreX());
					treasure.setCentreY(background.centreY());
					treasure.render(game.renderer);

					if (SDL_HasIntersection(&player.getCollisionRect(), &treasure.getCollisionRect()))
					{
						// end level if treasure picked up
						score += 10;
						levelInProgress = false;
					}
				}
				else {
					// BOSS BATTLE
					moveEnemies(currentRoom->enemyList, &player, currentRoom);
				}
			}

			// render player character
			int clipRectY = player.getDirectionFacing() * player.actorHeight();
			int clipRectX = player.getAnimationFrame() * player.actorWidth();
			SDL_Rect playerClipRect = { clipRectX, clipRectY, player.actorWidth(), player.actorHeight() };

			player.render(game.renderer, player.actorWidth(), player.actorHeight(), &playerClipRect);

			// deals with player attack animation
			if (playerAttacking)
			{
				getSwordLoc(player);
				SDL_Rect swordClipRect = { player.getDirectionFacing() * swordAttack.actorWidth(), 0, swordAttack.actorWidth(), swordAttack.actorHeight() };
				swordAttack.render(game.renderer, NULL, NULL, &swordClipRect);

				if (SDL_GetTicks() - playerAttackStart >= playerAttackLength)
				{
					swordAttack.setLocation(NULL, NULL);
					playerAttacking = false;
				}
			}

			// characters gains brief invincibility after taking damage, here we check how long they've been invincible and remove is nessecary
			player.checkInvincibilityTimer(thisLoopTime);

			for each (Character* c in currentRoom->enemyList)
			{
				c->checkInvincibilityTimer(thisLoopTime);
			}

			// checks enemy collision, calculate enemy health, delete dead enemies, render alive enemies, move enemies
			moveEnemies(currentRoom->enemyList, &player, currentRoom);

			// deal with projectiles
				// convert Projectiles objects to Charcter objects so that we can use the same function we used for enemies
			std::vector <Character*> convertedProjectiles;

			for (projectileIterator = currentRoom->projectileList.begin(); projectileIterator != currentRoom->projectileList.end(); projectileIterator++)
			{
				(*projectileIterator)->moveProjectile(thisLoopTime);
				convertedProjectiles.push_back(dynamic_cast <Character*> (*projectileIterator));
			}

			moveEnemies(convertedProjectiles, &player, currentRoom);

				//convert back to Projectiles
			currentRoom->projectileList.clear();
			for (enemyIterator = convertedProjectiles.begin(); enemyIterator != convertedProjectiles.end(); enemyIterator++)
			{
				currentRoom->projectileList.push_back(dynamic_cast <Projectile*> (*enemyIterator));
			}

			// display UI
			for (int i = 0; i < uiElements.size(); i++)
			{
				SDL_Rect clipRect = { uiElements[i].updateAnimationFrame(thisLoopTime) * uiElements[i].actorWidth(), 0, uiElements[i].actorWidth(), uiElements[i].actorHeight() };
				uiElements[i].render(game.renderer, NULL, NULL, &clipRect);
			}

			uiScoreText.loadTexture(game.renderer, std::to_string(score));
			uiScoreText.setLocation(uiScoreText.getCollisionRect().x, uiScoreText.setCentreY(uiScore.centreY()));
			uiScoreText.render(game.renderer);

			uiLifeText.loadTexture(game.renderer, std::to_string(player.health()));
			uiLifeText.setLocation(uiLifeText.getCollisionRect().x, uiLifeText.setCentreY(uiLife.centreY()));
			uiLifeText.render(game.renderer);


			SDL_RenderPresent(game.renderer);
		}

		// delete objects used in last level
		for (int i = roomList.size() - 1; i >= 0; i--)
		{
			for (int j = roomList[i].size() - 1; j >= 0; j--)
			{
				for (int k = roomList[i][j].enemyList.size() - 1; k >= 0; k--)
				{
					delete roomList[i][j].enemyList[k];
				}

				for (int k = roomList[i][j].projectileList.size() - 1; k >= 0; k--)
				{
					delete roomList[i][j].projectileList[k];
				}

				roomList[i][j].~Room();
			}
		}

		if (!gameOver)
		{
			level++;

			// boss difficulty up
			bossData.health++;

			// every level, either increase size of level or number of enemies
			if (level % 2 == 0)
			{
				maxEnemiesPerRoom++;
				levelDifficultyChanges.push_back("Number of Enemies UP");
			}

			else
			{
				floorsPerLevel++;
				roomsPerFloor++;
				levelDifficultyChanges.push_back("Level Size UP");
			}

			// every 5 levels increase zombie movement speed
			if (level % 5 == 0)
			{
				zombieData.moveDistance += 1;
				levelDifficultyChanges.push_back("Enemy Speed UP");
			}
		}
	}

	// game over instructions
	SDL_Delay(500);
	SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
	return updateHighscores(score);
}

void prelevelScreen(int level, int score, Actor scoreImg, int lives, Actor livesImg, std::vector<std::string> changes)
{
	// time variables
	Uint32 levelStartTime = SDL_GetTicks();
	Uint32 currentTime = levelStartTime;

	// UI objects
	Text levelNumText(fonts.pixelFont, white, 72, NULL, NULL, NULL, NULL, textData);
	levelNumText.loadTexture(game.renderer, "Level " + std::to_string(level));
	levelNumText.setLocation(levelNumText.setCentreX(background.centreX()), background.centreY() * 4 / 5);

	scoreImg.setLocation(background.actorWidth() * 5/11, background.actorHeight() / 2);
	Text scoreText(fonts.pixelFont, white, 48, NULL, NULL, NULL, NULL, textData);
	scoreText.loadTexture(game.renderer, std::to_string(score));
	scoreText.setLocation(scoreText.setCentreX(scoreImg.centreX()), scoreImg.getCollisionRect().y + scoreImg.actorHeight());

	livesImg.setLocation(background.actorWidth() * 6/11 - livesImg.actorWidth(), background.actorHeight() / 2);
	Text livesText(fonts.pixelFont, white, 48, NULL, NULL, NULL, NULL, textData);
	livesText.loadTexture(game.renderer, std::to_string(lives));
	livesText.setLocation(livesText.setCentreX(livesImg.centreX()), livesImg.getCollisionRect().y + livesImg.actorHeight());

	Text changesText(fonts.pixelFont, white, 42, NULL, NULL, NULL, NULL, textData);

	std::vector <Actor> ui = {scoreImg, livesImg};

	//
	// draw loop
	//
	while (currentTime - levelStartTime <= 2000)
	{
		SDL_SetRenderDrawColor(game.renderer, 123, 76, 0, 255);
		SDL_RenderClear(game.renderer);

		// image animation / render
		for (int i = 0; i < ui.size(); i++)
		{
			SDL_Rect clipRect = { ui[i].updateAnimationFrame(currentTime) * ui[i].actorWidth(), 0, ui[i].actorWidth(), ui[i].actorHeight() };
			ui[i].render(game.renderer, NULL, NULL, &clipRect);
		}

		// text render
		scoreText.render(game.renderer);
		livesText.render(game.renderer);
		levelNumText.render(game.renderer);

		// displays how the difficulty has changed since last level
		for (int i = 0; i < changes.size(); i++)
		{
			changesText.loadTexture(game.renderer, changes[i]);
			changesText.setLocation(changesText.setCentreX(background.centreX()), scoreText.getCollisionRect().y + scoreText.getCollisionRect().h + (i+1) * changesText.getCollisionRect().h);
			changesText.render(game.renderer);
		}

		currentTime = SDL_GetTicks();

		SDL_RenderPresent(game.renderer);
	}

}

bool updateHighscores(int thisGameScore)
{
	// constants
	const int numOfHighscores = 10;
	const int nameLength = 3;

	// file handling variables
	std::ifstream highscoresFile;
	std::ofstream newHighscoresFile;

	// vectors
	std::vector<highscore> highscores;
	std::vector<highscore> newHighscores;
	std::vector<std::string> names;
	std::vector<int> scores;

	// other variables
	int newScorePlacement;
	std::string inFileName;
	int inFileScore;
	int letterRenderWidth = background.actorWidth() / 15;

	highscoresFile.open(highscoresFileName, std::ios::in);

	// if highscores file doesn't exist, create one with default entries
	if (!highscoresFile.good())
	{
		names = { "aaa", "bbb", "ccc", "ddd", "eee", "fff", "ggg", "hhh", "iii", "jjj" };
		scores = { 100, 90, 80, 70, 60, 50, 40, 30, 20, 10 };

		for (int i = 0; i < numOfHighscores; i++)
		{
			if (i < scores.size() && i < names.size())
			{
				highscores.push_back({ names[i], scores[i], false });
			}
		}
	}

	// read highscores file
	else
	{
		while (highscoresFile >> inFileName >> inFileScore)
		{
			highscores.push_back({ inFileName, inFileScore, false });
		}
	}

	highscoresFile.close();


	for (int i = 0; i < highscores.size(); i++)
	{
		// check score from last game against highscores
		if (thisGameScore >= highscores[i].score)
		{
			// new highscore
			highscore newHighscore;
			newHighscore.score = thisGameScore;
			newHighscore.newHighscore = true;

			newScorePlacement = i;

			std::string name = "   ";

			// text objects
			Text newHSText(fonts.pixelFont, gold, 48, NULL, NULL, NULL, NULL, textData);
			Text enterName(fonts.miniFont, white, 36, NULL, NULL, NULL, NULL, textData);
			Text letterText(fonts.miniFont, white, 72, NULL, NULL, NULL, NULL, textData);

			SDL_Event e;

			// enter name
			for (int j = 0; j < nameLength; j++)
			{
				// enter letter
				bool newEntryRequired = true;
				while (newEntryRequired)
				{
					SDL_SetRenderDrawColor(game.renderer, 123, 76, 0, 255);
					SDL_RenderClear(game.renderer);

					// render texts
					newHSText.loadTexture(game.renderer, "NEW HIGHSCORE!");
					newHSText.setLocation(newHSText.setCentreX(background.centreX()), background.actorHeight() / 3);
					newHSText.render(game.renderer);

					enterName.loadTexture(game.renderer, "Please enter your name");
					enterName.setLocation(enterName.setCentreX(background.centreX()), newHSText.getCollisionRect().y + newHSText.actorHeight() * 2);
					enterName.render(game.renderer);

					while (SDL_PollEvent(&e))
					{
						switch (e.type)
						{
						case SDL_QUIT:
							return true;
							break;

						case SDL_KEYDOWN:
							if (!e.key.repeat)
							{
								// user enters a letter
								name[j] = e.key.keysym.sym;
								newEntryRequired = false;
							}
							break;

						}
					}

					// render each letter
					for (int k = 0; k <= j; k++)
					{
						letterText.loadTexture(game.renderer, std::string(1, name[k]));
						letterText.setLocation(background.actorWidth() * (k + 4) / 10 - letterRenderWidth / 2, enterName.getCollisionRect().x + enterName.actorHeight());
						letterText.render(game.renderer, letterRenderWidth);

						// draw an underline under where the next letter is to be entered
						if (k == j)
						{
							SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
							SDL_RenderDrawLine(game.renderer, letterText.getCollisionRect().x, letterText.getCollisionRect().y + letterText.actorHeight(), letterText.getCollisionRect().x + letterRenderWidth, letterText.getCollisionRect().y + letterText.actorHeight());
						}
					}

					SDL_RenderPresent(game.renderer);
				}
			}

			// add highscore to highscores vector
			newHighscore.name = name;
			highscores.insert(highscores.begin() + i, newHighscore);

			SDL_Delay(500);
			break;
		}	
	}

	// update highscores file
	newHighscoresFile.open(highscoresFileName, std::ios::trunc);

	for (int i = 0; i < numOfHighscores; i++)
	{
		if (i < highscores.size())
		{
			newHighscores.push_back(highscores[i]);
			newHighscoresFile << highscores[i].name << " " << highscores[i].score << '\n';
		}
	}

	newHighscoresFile.close();

	return displayHighscores(newHighscores);
}

bool displayHighscores(std::vector<highscore> highscores)
{
	bool showHighscores = true;
	SDL_Event e;

	// text objects
	Text title(fonts.miniFont, gold, 72, NULL, NULL, NULL, NULL, textData);
	title.loadTexture(game.renderer, "HIGHSCORES");
	title.setLocation(title.setCentreX(background.centreX()), background.actorHeight() / 30);

	Text tContinue(fonts.pixelFont, gold, 48, NULL, NULL, title.actorWidth(), NULL, textData);
	tContinue.loadTexture(game.renderer, "Press any key to continue...");
	tContinue.setLocation(tContinue.setCentreX(background.centreX()), background.actorHeight() * 12 / 13);

	// if a new highscore, shows in different colour
	Text oldHighscore(fonts.pixelFont, white, 48, NULL, NULL, NULL, NULL, textData);
	Text newHighscore(fonts.pixelFont, gold, 48, NULL, NULL, NULL, NULL, textData);

	SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);

	while (showHighscores)
	{
		SDL_SetRenderDrawColor(game.renderer, 123, 76, 0, 255);
		SDL_RenderClear(game.renderer);

		// display each highscore
		for (int i = 0; i < highscores.size(); i++)
		{
			// set reference to text object we need to use, depending on whether highscore is new
			Text* hsText;
			(highscores[i].newHighscore) ? hsText = &newHighscore : hsText = &oldHighscore;

			// render name
			hsText->loadTexture(game.renderer, highscores[i].name);
			hsText->setLocation(background.actorWidth() / 3, background.actorHeight() * (i + 2) / 13);
			hsText->render(game.renderer);

			// render score
			hsText->loadTexture(game.renderer, std::to_string(highscores[i].score));
			hsText->setLocation(background.actorWidth() * 2 / 3 - hsText->actorWidth(), hsText->getCollisionRect().y);
			hsText->render(game.renderer);
		}

		// render other text objects
		tContinue.render(game.renderer);
		title.render(game.renderer);

		SDL_RenderPresent(game.renderer);

		// use wait event instead of poll event since there will be no changes to the display and we don't need to loop
		if (SDL_WaitEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return true;
			}

			else if (e.type == SDL_KEYDOWN && !e.key.repeat)
			{
				showHighscores = false;
			}
		}

	}

	return false;
}

void readDifficultyFile(int iFile[], int arrSize)
{

	// default values	{floorsPerLevel, roomsPerFloor, enemiesPerRoom, playerMovespeed, enemyBonusSpeed}
	int defaultValues[] = { 2, 2, 2, 5, 0 };

	std::ifstream difficulyFile;
	difficulyFile.open(difficultyFileName, std::ios::in);

	if (!difficulyFile.good())
	{
	// if file doesn't exist, set default values

		iFile[0] = defaultValues[0]; // floors per level
		iFile[1] = defaultValues[1]; // maximum rooms per floor
		iFile[2] = defaultValues[2]; // maximum enemies per room
		iFile[3] = defaultValues[3]; // player movement speed
		iFile[4] = defaultValues[4]; // enemy bonus movement speed (added to base 2 speed)

	// and then create file
		std::ofstream newDifficultyFile;
		newDifficultyFile.open(difficultyFileName, std::ios::out);

		for (int i = 0; i < arrSize; i++)
		{
			newDifficultyFile << iFile[i] << '\n';
		}

		newDifficultyFile.close();
	}

	// read difficuly file
	else
	{
		for (int i = 0; i < arrSize; i++)
		{
			std::string line;
			std::getline(difficulyFile, line);
			if (line != "")
			{
				int variable;
				try
				{
					variable = std::stoi(line);
				}
				catch(...)
				{
					variable = defaultValues[i];
				}
				iFile[i] = variable;
			}
		}

		difficulyFile.close();
	}
}

void getSwordLoc(Player player)
{
	// sets sword location based on which way the player is currently facing

	int swordX = NULL, swordY = NULL;

	switch(player.getDirectionFacing())
	{
	case UP_FACE:
		swordY = player.getCollisionRect().y - swordAttack.actorHeight();

	case DOWN_FACE:
		swordX = player.centreX() - swordAttack.actorWidth() / 2;
		if (swordY == NULL)
		{
			swordY = player.getCollisionRect().y + player.actorHeight();
		}
		break;

	case LEFT_FACE:
		swordX = player.getCollisionRect().x - swordAttack.actorWidth();

	case RIGHT_FACE:
		swordY = player.centreY() - swordAttack.actorHeight() / 2;
		if (swordX == NULL)
		{
			swordX = player.getCollisionRect().x + player.actorWidth();
		}
		break;
	}

	swordAttack.setLocation(swordX, swordY);
}

void createDoorFillRects(std::vector<SDL_Rect> *walls, Room *r, int gapWidth, int gapHeight)
{
	// put collision rects over unused doors to stop player from moving through them

	int wallWidth = tileWidth * 2;
	int wallHeight = tileHeight * 2;

	int rightWallX = background.actorWidth() - wallWidth;
	int botWallY = background.actorHeight() - wallHeight;

	const int MINIMUM_NUMBER_OF_WALLS = 8;

	// remove rects created from previous room
	// should be 8 walls by default!
	while (walls->size() > MINIMUM_NUMBER_OF_WALLS)
	{
		walls->pop_back();
	}

	if (r->location().leftmost)
	{
		walls->push_back(SDL_Rect{ 0, background.centreY() - gapHeight / 2, wallWidth, gapHeight });
	}

	if (r->location().rightmost)
	{
		walls->push_back(SDL_Rect{ rightWallX, background.centreY() - gapHeight / 2, wallWidth, gapHeight });
	}

	if (!r->location().entrance || r->getFloorNum() <= 0)
	{
		walls->push_back(SDL_Rect{ background.centreX() - gapWidth / 2, 0, gapWidth, wallHeight });
	}

	if (!r->location().exit)
	{
		walls->push_back(SDL_Rect{ background.centreX() - gapWidth / 2, botWallY, gapWidth, wallHeight });
	}

}

void coverUnusedDoors(Room* currentRoom)
{
	// render image over unused doors

	int tbX = background.actorWidth() / 2 - tileWidth * 2; // top, bottom x
	int lrY = background.actorHeight() / 2 - tileHeight * 2; // left, right y

	if (!currentRoom->location().entrance || currentRoom->getFloorNum() == 0)
	{
		tbWallFill.setLocation(tbX, 0);
		tbWallFill.render(game.renderer);
	}

	if (!currentRoom->location().exit)
	{
		tbWallFill.setLocation(tbX, ceil(background.actorHeight() - tileHeight * 2));
		tbWallFill.render(game.renderer, NULL, NULL, NULL, NULL, SDL_FLIP_VERTICAL);
	}

	if (currentRoom->location().leftmost)
	{
		lrWallFill.setLocation(0, lrY);
		lrWallFill.render(game.renderer);
	}

	if (currentRoom->location().rightmost)
	{
		lrWallFill.setLocation(ceil(background.actorWidth() - tileWidth * 2), lrY);
		lrWallFill.render(game.renderer, NULL, NULL, NULL, NULL, SDL_FLIP_HORIZONTAL);
	}
}

void createConstCollisionRects(std::vector<SDL_Rect> *doors, std::vector<SDL_Rect> *walls, int gapWidth, int gapHeight)
{
	// collision rects that are present in every room, therefore do not change during the game loop

	// door rects used to change between rooms
	doors->clear();

	// wall rects keep player inside play area
	walls->clear();

	int wallWidthLongSide = tileWidth * 2;
	int wallWidthShortSide = background.centreX() - gapWidth / 2;

	int wallHeightLongSide = tileHeight * 2;
	int wallHeightShortSide = background.centreY() - gapHeight / 2;

	int rightWallX = background.actorWidth() - wallWidthLongSide;
	int botWallY = background.actorHeight() - wallHeightLongSide;

	// left door
	doors->push_back({ 0, background.centreY() - gapHeight / 2, wallWidthLongSide / 4, gapHeight });

	// left walls
	walls->push_back(SDL_Rect{ 0, 0, wallWidthLongSide, background.centreY() - gapHeight / 2 });
	walls->push_back(SDL_Rect{ 0, background.centreY() + gapHeight / 2, wallWidthLongSide, background.centreY() - gapHeight / 2 });


	// right door
	doors->push_back({ background.actorWidth() - wallWidthLongSide / 4, background.centreY() - gapHeight / 2, wallWidthLongSide / 4, gapHeight });

	// right walls
	walls->push_back(SDL_Rect{ rightWallX, 0, wallWidthLongSide, wallHeightShortSide });
	walls->push_back(SDL_Rect{ rightWallX, background.centreY() + gapHeight / 2, wallWidthLongSide, wallHeightShortSide });


	// top door
	doors->push_back({ background.centreX() - gapWidth / 2, 0, gapWidth, wallHeightLongSide / 4 });

	// top walls
	walls->push_back(SDL_Rect{ 0, 0, wallWidthShortSide, wallHeightLongSide });
	walls->push_back(SDL_Rect{ background.centreX() + gapWidth / 2, 0, wallWidthShortSide, wallHeightLongSide });

	// bottom door
	doors->push_back({ background.centreX() - gapWidth / 2, background.actorHeight() - wallHeightLongSide / 4, gapWidth, wallHeightLongSide / 4 });

	// bottom walls
	walls->push_back(SDL_Rect{ 0, botWallY, wallWidthShortSide, wallHeightLongSide });
	walls->push_back(SDL_Rect{ background.centreX() + gapWidth / 2, botWallY, wallWidthShortSide, wallHeightLongSide });

}

std::vector <std::vector <Room>> generateLevelRooms(int roomsPerFloor, int floorsPerLevel, int maxEnemies)
{
	// returns vector of vector of Room objects

	std::vector <std::vector <Room>> roomList(floorsPerLevel);

	game.generateRandSeed();

	// "nessecary" room numbers -> the number of rooms from floor entrance to floor exit
	int roomRangeLowest;
	int roomRangeHighest;

	// used to calculate how many "unnessecary" rooms to add
	int lowestToZero;
	int highestToLimit;

	// number of room furthest left and furthest right on floor
	int leftmost;
	int rightmost;

	int entranceRoom = rand() % roomsPerFloor;
	int exitRoom;

	for (int i = 0; i < floorsPerLevel; i++) 
	{
		// create Room objects
		for (int j = 0; j < roomsPerFloor; j++)
		{
			Room newRoom;
			roomList[i].push_back(newRoom);
			roomList[i][j].setFloor(i);
			roomList[i][j].setRoom(j);
		}

		// set random room on floor to be entrance
		roomList[i][entranceRoom].setEntrance();

		// set random exit room that is at least a certain distance away from entrance room
		do {
			exitRoom = rand() % roomsPerFloor;
		} while (exitRoom == entranceRoom || abs(exitRoom - entranceRoom) < roomsPerFloor / 2 - 1);

		if (i == floorsPerLevel - 1)
		{
			roomList[i][exitRoom].setBossRoom();

			Character* boss = new Character(0, 0, background.actorWidth() / 12, background.actorHeight() / 5, bossData);
			boss->setLocation(background.centreX() - boss->actorWidth() / 2, background.centreY() - boss->actorHeight() / 2);

			roomList[i][exitRoom].enemyList.push_back(boss);


		}
		else
		{
			roomList[i][exitRoom].setExit();
		}

		// calculate nessecary rooms
		if (exitRoom < entranceRoom)
		{
			roomRangeLowest = exitRoom;
			roomRangeHighest = entranceRoom;
		}
		else
		{
			roomRangeLowest = entranceRoom;
			roomRangeHighest = exitRoom;
		}

		// calculate how many extra rooms are on each side of the nessecary rooms
		lowestToZero = roomRangeLowest;
		highestToLimit = (roomsPerFloor - 1) - roomRangeHighest;

		// if the range from lowest nessacery room to 0 is 0, there are no more rooms to add on that side
		if (lowestToZero == 0)
		{
			leftmost = 0;
		}
		else
		{
			// otherwise add a random amount of extra rooms
			leftmost = rand() % lowestToZero;
		}
		
		roomList[i][leftmost].setLeftmost();

		// and set the others to unused
		for (int j = 0; j < leftmost; j++)
		{
			roomList[i][j].setUnused();
		}

		// same as above but for the other side
		if (highestToLimit == 0)
		{
			rightmost = roomsPerFloor - 1;
		}
		else
		{
			// extra rooms
			rightmost = roomRangeHighest + (rand() % highestToLimit);
		}

		roomList[i][rightmost].setRightmost();

		for (int j = rightmost + 1; j < roomsPerFloor; j++)
		{
			// set unused rooms
			roomList[i][j].setUnused();
		}

		// set up rooms that we are using
		for (int j = leftmost; j <= rightmost; j++)
		{
			roomList[i][j].setDoorLocations();
			
			// create enemies in each room
			if (!(i == 0 && roomList[i][j].location().entrance) && !roomList[i][j].isUnused() && !roomList[i][j].bossRoom())
			{
				int numOfEnemies = rand() % maxEnemies;
				numOfEnemies++;
				createEnemies(roomList[i][j].enemyList, numOfEnemies);
			}
		}

	}
	return roomList;
}

bool checkWallCollisions(std::vector <SDL_Rect> walls, Character& c)
{
	// returns true if unit has collided
	bool collided = false;

	// returns a rect for each side of object
	std::vector <SDL_Rect> sides = c.getMoreCollisionRects();

	SDL_Rect overlapRect;

	for (SDL_Rect rect : walls)
	{
		if (SDL_HasIntersection(&rect, &c.getCollisionRect()))
		{
			collided = true;
			// check that the opposite side is NOT colliding at the same time, if it is that means that one of the other 2 sides is the actual side colliding
				// e.g. if top and bottom are both colliding, either the left or right is the actual side colliding
			while (SDL_IntersectRect(&rect, &sides[0], &overlapRect) && !SDL_HasIntersection(&rect, &sides[1]))
			{
				// set location of object away from wall
				c.setLocation(c.getCollisionRect().x, c.getCollisionRect().y + overlapRect.h);
				sides = c.getMoreCollisionRects(); // update collision rects, otherwise get stuck in while loop!
			}
			while (SDL_IntersectRect(&rect, &sides[1], &overlapRect) && !SDL_HasIntersection(&rect, &sides[0]))
			{
				c.setLocation(c.getCollisionRect().x, c.getCollisionRect().y - overlapRect.h);
				sides = c.getMoreCollisionRects();
			}
			while (SDL_IntersectRect(&rect, &sides[2], &overlapRect) && !SDL_HasIntersection(&rect, &sides[3]))
			{
				c.setLocation(c.getCollisionRect().x + overlapRect.w, c.getCollisionRect().y);
				sides = c.getMoreCollisionRects();
			}
			while (SDL_IntersectRect(&rect, &sides[3], &overlapRect) && !SDL_HasIntersection(&rect, &sides[2]))
			{
				c.setLocation(c.getCollisionRect().x - overlapRect.w, c.getCollisionRect().y);
				sides = c.getMoreCollisionRects();
			}
		}
	}
	return collided;
}

Room* checkDoorCollisions(Player *p, std::vector <SDL_Rect> *doors, Room *r, std::vector<std::vector<Room>> &roomList, int doorWidth, int doorHeight)
{
	// set location of player if they go through door

	int currentRoom = r->getRoomNum();
	int currentFloor = r->getFloorNum();

	for (int i = 0; i < doors->size(); i++)
	{
		if (SDL_HasIntersection(&(*p).getCollisionRect(), &(*doors)[i]))
		{
			// left door
			if (i == 0 && !r->location().leftmost)
			{
				currentRoom--;
				(*p).setLocation(background.actorWidth() - (*p).actorWidth() - (*doors)[i].w, background.centreY());
			}

			// right door
			if (i == 1 && !r->location().rightmost)
			{
				currentRoom++;
				(*p).setLocation((*p).actorWidth() + (*doors)[i].w, background.centreY());
			}

			// top door
			if (i == 2 && r->location().entrance)
			{
				currentFloor--;
				(*p).setLocation(background.centreX(), background.actorHeight() - (*p).actorHeight() - (*doors)[i].h);

				for (int j = 0; j < roomList[currentFloor].size(); j++)
				{
					if (roomList[currentFloor][j].location().exit)
					{
						currentRoom = j;
						break;
					}
				}
			}

			// bottom door
			if (i == 3 && r->location().exit)
			{
				currentFloor++;
				(*p).setLocation(background.centreX(), (*p).actorHeight() + (*doors)[i].h);

				for (int j = 0; j <  roomList[currentFloor].size(); j++)
				{
					if (roomList[currentFloor][j].location().entrance)
					{
						currentRoom = j;
						break;
					}
				}
			}

			// unload textures of enemies from room player is leaving
			for (int j = 0; j < r->enemyList.size(); j++)
			{
				r->enemyList[j]->unloadTexture();
			}

			// sets up new room
			r = changeRoom(roomList, currentFloor, currentRoom, doorWidth, doorHeight);

			break;

		}
	}

	return r;
}

Room* changeRoom(std::vector<std::vector<Room>> &roomList, int floor, int room, int doorWidth, int doorHeight)
{
	// sets up room that player enters

	// reference to NEW room
	Room* r;
	r = &roomList[floor][room];

	// load enemies
	for (enemyIterator = r->enemyList.begin(); enemyIterator != r->enemyList.end(); enemyIterator++)
	{
		(*enemyIterator)->setLastAttackTime(SDL_GetTicks());
		(*enemyIterator)->loadTexture(game.renderer);
	}

	// load projectiles
	for (projectileIterator = r->projectileList.begin(); projectileIterator != r->projectileList.end(); projectileIterator++)
	{
		(*projectileIterator)->loadTexture(game.renderer);
	}

	// collision rects for new door locations
	createDoorFillRects(&wallCollisionRects, r, doorWidth, doorHeight);

	return r;
}

void createEnemies(std::vector<Character*>& e, int numberOfEnemies)
{
	// populate rooms with enemies

	for (int i = 0; i < numberOfEnemies; i++)
	{
		Character* newEnemy;
		int randomEnemy = rand() & 2;

		if (randomEnemy == 0)
		{
			// squid enemy
			newEnemy = new Character (0, 0, NULL, NULL, squidData);
		}
		else
		{
			// zombie enemy
			newEnemy = new Character(0, 0, NULL, NULL, zombieData);
		}

		e.push_back(newEnemy);

		// set random start location
		int randomX = rand() % (playableArea.w);
		randomX += playableArea.x;

		int randomY = rand() % (playableArea.h);
		randomY += playableArea.y;

		e.back()->setLocation(randomX, randomY);
		
	}
}

void moveEnemies(std::vector<Character*>& e, Player *p, Room *r)
{
	MOVEMENT_DIRECTION moveX, moveY;
	bool collidedWithWall;

	for (enemyIterator = e.begin(); enemyIterator != e.end();)
	{

		// enemy comes into contact with player
		if (SDL_HasIntersection(&(*enemyIterator)->getCollisionRect(), &p->getCollisionRect()) && !SDL_HasIntersection(&(*enemyIterator)->getCollisionRect(), &swordAttack.getCollisionRect()))
		{
			// damage player
			p->takeDamage(1, thisLoopTime);

			// if projectile hits player, also damages projectile
			if ((*enemyIterator)->getID() == PROJECTILE)
			{
				(*enemyIterator)->takeDamage(1, thisLoopTime);
			}
		}

		// enemy hit by sword
		if (playerAttacking && SDL_HasIntersection(&swordAttack.getCollisionRect(), &(*enemyIterator)->getCollisionRect()))
		{
			(*enemyIterator)->takeDamage(1, thisLoopTime);
		}

		// check wall collisions
		collidedWithWall = checkWallCollisions(wallCollisionRects, **enemyIterator);

			// damage projectiles that hit walls
		if (collidedWithWall && (*enemyIterator)->getID() == PROJECTILE)
		{
			(*enemyIterator)->takeDamage(1, thisLoopTime);
		}

		//delete dead enemies
		if ((*enemyIterator)->health() <= 0)
		{
			if ((*enemyIterator)->getID() == ZOMBIE || (*enemyIterator)->getID() == SQUID)
			{
				score++;
			}

			enemyIterator = e.erase(enemyIterator);
			continue;
		}

		//render enemy

			// changes direction enemy is facing
		int spritesheetY = 0;

		if (p->getCollisionRect().x >= (*enemyIterator)->getCollisionRect().x && (*enemyIterator)->getID() != PROJECTILE) // projectiles don't have directional sprites so exclude them
		{
			spritesheetY = (*enemyIterator)->actorHeight();
		}

		SDL_Rect rect = { (*enemyIterator)->getAnimationFrame() * (*enemyIterator)->actorWidth(), spritesheetY, (*enemyIterator)->actorWidth(), (*enemyIterator)->actorHeight() };
		if ((*enemyIterator)->getID() == BOSS)
		{
			// render boss enemy without clipRect, is causes it to render wrong
			(*enemyIterator)->render(game.renderer);
		}
		else {
		(*enemyIterator)->render(game.renderer, NULL, NULL, &rect);
		}

		switch ((*enemyIterator)->getID())
		{
		case ZOMBIE:
			// moves towards the player
				// if above or left of player, move positive. If below or right of player move negative
			moveX = (*enemyIterator)->calcMoveDirection(p->centreX(), (*enemyIterator)->getCollisionRect().x + (*enemyIterator)->actorWidth(), (*enemyIterator)->getCollisionRect().x);
			moveY = (*enemyIterator)->calcMoveDirection(p->centreY(), (*enemyIterator)->getCollisionRect().y + (*enemyIterator)->actorHeight(), (*enemyIterator)->getCollisionRect().y);
			(*enemyIterator)->moveCharacter(moveX, moveY, NULL, thisLoopTime);
			break;

		case SQUID:
			// this enemy doesn't move, just update animation instead
			(*enemyIterator)->updateAnimationFrame(thisLoopTime);

			//create projectiles
			if (thisLoopTime - (*enemyIterator)->getLastAttackTime() >= 3000)
			{
				(*enemyIterator)->setLastAttackTime(thisLoopTime);
				Projectile* newProj = new Projectile((*enemyIterator)->centreX(), (*enemyIterator)->centreY(), NULL, NULL, squidProjectileData, p->centreX(), p->centreY());
				r->projectileList.push_back(newProj);
				r->projectileList.back()->loadTexture(game.renderer);
				r->projectileList.back()->setLocation(r->projectileList.back()->setCentreX((*enemyIterator)->centreX()), r->projectileList.back()->setCentreY((*enemyIterator)->centreY()));
			}
			break;
		case BOSS:
			Character* boss = r->enemyList[0];

			int numOfProjectiles = level + 3;

			if (thisLoopTime - boss->getLastAttackTime() >= 1500)
			{
				bool targettedPlayer = false;
				Projectile playerTargettingProjectile(boss->centreX(), boss->centreY(), NULL, NULL, squidProjectileData, p->centreX(), p->centreY());
				for (int i = 0; i < numOfProjectiles; i++)
				{
					//create projectile		
					int projectileAngle = rand() % 360;
					int projTargetX, projTargetY;

					// one projectile of every volley directly targets the player
					if (!targettedPlayer)
					{
						projTargetX = p->centreX();
						projTargetY = p->centreY();
						targettedPlayer = true;
					}
					else
					{
						projTargetX = background.actorWidth() * sin(projectileAngle);
						projTargetY = background.actorHeight() * cos(projectileAngle);
					}

					Projectile* p = new Projectile(boss->centreX(), boss->centreY(), NULL, NULL, squidProjectileData, projTargetX, projTargetY);
					r->projectileList.push_back(p);
					r->projectileList.back()->loadTexture(game.renderer);
					r->projectileList.back()->setLocation(r->projectileList.back()->setCentreX(boss->centreX()), r->projectileList.back()->setCentreY(boss->centreY()));
				}

				boss->setLastAttackTime(thisLoopTime);
			}
		}

		//iterate at the end of the loop, this way it doesn't iterate if we delete an enemy from the vector
		enemyIterator++;
	}
}