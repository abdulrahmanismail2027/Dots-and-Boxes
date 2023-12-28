#pragma once
#include "constants.h"
#include "datastruct.h"
void printArray(int arr[], char size);
void printMatrix(char gridSize, char grid[][MAX_GRID_SIZE]);
void printGameInfo(gameInfo game,Names playerNames, int time);
void printGrid(char gridSize, char grid[][MAX_GRID_SIZE]);
void initializeGrid(char gridSize, char grid[][MAX_GRID_SIZE]);