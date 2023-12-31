#include "constants.h"
#include <math.h>
#include <stdlib.h>
void splitMove(int value,char *tr1,char *tc1,char *tr2,char *tc2) // Splits coordinate into 4 separate coordinates
{
    *tr1=(value/1000); *tc1=(value/100%10); *tr2=(value/10%10); *tc2=(value%10);
}
char demap(char i) // Demaps a single coordinate
{
    i = i/2 + 1;
    return i;
}
int inRange(char r1, char c1, char r2, char c2, char d) // Checks that the move is in the appropriate range
{
    if ((r1<=d && r1>0) && (r2<=d && r2>0) && (c1<=d && c1>0) && (c2<=d && c2>0))
        return 1; // In range
    else
        return 0; // Not in range
}
int validMove(char r1, char c1, char r2, char c2, char gridSize) // Validates the feasibility of the move game-wise
{
    // Checks if the move is feasible
    if (!((r1==r2 && (int)fabs(c1-c2)==1) || (c1==c2 && (int)fabs(r1-r2)==1)))
    {
        return 0; // Invalid move
    }
    // Checks if the move is in range
    else if (!inRange(r1, c1, r2, c2, (gridSize+1)/2))
    {
        return 0;
    }
    else
    {
        return 1; // Valid move
    }
    }
int validCell(char r1, char c1, char r2, char c2, char gridSize,char grid[][gridSize]) // Checks cell's occupancy
{
    // Checks if the cell is empty
    if ((r1 == r2) && grid[r1][(int)fmin(c1, c2)+1])
    {
        return 0; // Invalid move (cell is occupied)
    }
    else if ((c1 == c2) && (grid[(int)fmin(r1, r2)+1][c1]))
    {
        return 0;
    }
    else
    {
        return 1; // Valid move (cell is empty)
    }
}
void generateMove(char *r1, char *c1, char *r2, char *c2)
{
    int num = (rand() % (9999 - 1000 + 1)) + 1000;
    splitMove(num, r1, c1, r2, c2);
}
void mapMove(char *r1, char *c1, char *r2, char *c2) // Maps coordinates onto grid indices
{
    *r1=2 * *r1 - 2; *r2=2 * *r2 - 2; *c1=2 * *c1 - 2; *c2=2 * *c2 - 2;
}
void realizeMove(char r1, char c1, char r2, char c2,char gridSize, char grid[][gridSize],char playerTurn) // Registers move in the grid
{
    if (r1 == r2)
    {
        if (grid[r1][(int)fmin(c1, c2)+1] != chain) grid[r1][(int)fmin(c1, c2)+1] = playerTurn;
    }
    else
    {
        if (grid[(int)fmin(r1, r2)+1][c1] != chain) grid[(int)fmin(r1, r2)+1][c1] = playerTurn;
    }
}
