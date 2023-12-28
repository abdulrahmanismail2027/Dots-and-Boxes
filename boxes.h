#pragma once
# include "constants.h"
int dfs(char r, char c, int *dfsStackIndex, int dfsStack[], char gridSize, char grid[][MAX_GRID_SIZE]);
int checkBox(char r1, char c1, char r2, char c2,char gridSize, char grid[][MAX_GRID_SIZE],char playerTurn ,char *remainingMoves);
void changeBoxColor(char r, char c,char playerTurn, char gridSize, char grid[][MAX_GRID_SIZE]);
int validBox(char r, char c, char gridSize, char grid[][MAX_GRID_SIZE]);
int visited(char r, char c, int dfsStackIndex, int dfsStack[]);
int connectedDot(char r, char c, char gridSize, char grid[][MAX_GRID_SIZE]);
void markBoxes(int stack[],int numOfBoxes, char gridSize, char grid[][MAX_GRID_SIZE]);
void splitBoxCoordinates(int coordinate, char *r, char *c);