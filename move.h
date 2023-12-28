#pragma once
#include "constants.h"
void splitMove(int value,char *tr1,char *tc1,char *tr2,char *tc2); // Splits coordinate into 4 separate coordinates
char demap(char i); // Demaps a single coordinate
int inRange(char r1, char c1, char r2, char c2, char d); // Checks that the move is in the appropriate range
int validMove(char r1, char c1, char r2, char c2, char gridSize); // Validates the feasibility of the move game-wise
int validCell(char r1, char c1, char r2, char c2, char gridSize,char grid[][MAX_GRID_SIZE]); // Checks cell's occupancy
void generateMove(char *r1, char *c1, char *r2, char *c2);
void mapMove(char *r1, char *c1, char *r2, char *c2); // Maps coordinates onto grid indices
void realizeMove(char r1, char c1, char r2, char c2,char gridSize, char grid[][MAX_GRID_SIZE],char playerTurn); // Registers move in the grid