#pragma once
#include "datastruct.h"
#include "constants.h"
void dumpGrid(char gridSize, char stackSize, char grid[][MAX_GRID_SIZE],char gridStack[][MAX_GRID_SIZE][MAX_STACK_SIZE + 1],char stackIndex);
void popGrid(char gridSize, char stackSize, char grid[][MAX_GRID_SIZE],char gridStack[][MAX_GRID_SIZE][MAX_STACK_SIZE + 1],char stackIndex);
void dumpGameInfo(gameInfo Game, gameInfo gameInfoStack[], char stackIndex);
void popGameInfo(gameInfo *Game, gameInfo gameInfoStack[], char stackIndex);
char saveGame(char gridSize, char grid[][MAX_GRID_SIZE], gameInfo gameInfo, Names playerNames, char saveCounter, int elapsedTime);
gameInfo loadStruct(int instanceChoice);
void loadGrid(char gridSize, char grid[][MAX_STACK_SIZE + 1],int instanceChoice);
Names loadNames();
int loadTime();

