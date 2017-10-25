#pragma once
#include "Game.h"
#include "Room.h"
#include "Player.h"
#include "Text.h"
#include <iostream>
#include <fstream>

void startMenu();

bool mainGame();
void prelevelScreen(int level, int score, Actor scoreImg, int lives, Actor livesImg, std::vector<std::string> changes);

bool updateHighscores(int thisGameScore);
bool displayHighscores(std::vector<highscore> highscores);

void readDifficultyFile(int iFile[], int arrSize);

std::vector <std::vector <Room>> generateLevelRooms(int roomsPerFloor, int floorsPerLevel, int maxEnemies);

void createConstCollisionRects(std::vector<SDL_Rect> *doors, std::vector<SDL_Rect> *walls, int gapWidth, int gapHeight);

void createDoorFillRects(std::vector<SDL_Rect> *walls, Room *r, int gapWidth, int gapHeight);
void coverUnusedDoors(Room* currentRoom);

bool checkWallCollisions(std::vector <SDL_Rect> walls, Character& c);
Room* checkDoorCollisions(Player *p, std::vector <SDL_Rect> *doors, Room *r, std::vector<std::vector<Room>> &roomList, int doorWidth, int doorHeight);

Room* changeRoom(std::vector<std::vector<Room>> &roomList, int floor, int room, int doorWidth, int doorHeight);

void createEnemies(std::vector<Character*>& e, int numberOfEnemies);
void moveEnemies(std::vector<Character*>& e, Player *p, Room *r);

void getSwordLoc(Player player);