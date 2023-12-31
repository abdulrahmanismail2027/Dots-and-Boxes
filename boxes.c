#include "constants.h"
#include "managearrs.h"
#include <math.h>
#include <stdio.h>
void splitBoxCoordinates(int coordinate, char *r, char *c)
{
    *c = (char)coordinate%10;
    *r = (char)coordinate/10;
    return;
}
void markBoxes(int stack[],int numOfBoxes, char gridSize, char grid[][gridSize])
{
    char r, c;
    int i;
    for (i = 0; i < numOfBoxes; i++)
    {
        splitBoxCoordinates(stack[i], &r, &c);
        if (!grid[r+1][c]) grid[r+1][c] = chain;
        if (!grid[r-1][c]) grid[r-1][c] = chain;
        if (!grid[r][c-1]) grid[r][c-1] = chain;
        if (!grid[r][c+1]) grid[r][c+1] = chain;
    }
}
void changeBoxColor(char r, char c,char playerTurn, char gridSize, char grid[][gridSize])
{
    if (grid[r+1][c] != 2) grid[r+1][c] = playerTurn;
    if (grid[r-1][c] != 2) grid[r-1][c] = playerTurn;
    if (grid[r][c-1] != 2) grid[r][c-1] = playerTurn;
    if (grid[r][c+1] != 2) grid[r][c+1] = playerTurn;
}
int connectedDot(char r, char c, char gridSize, char grid[][gridSize])
{
    if (r == 0)
    {
        if (c == 0)
        {
            return (grid[r+1][c] && grid[r][c+1]);
        }
        else if (c == gridSize - 1)
        {
            return (grid[r+1][c] && grid[r][c-1]);
        }
        else if (c > 0 && c < gridSize - 1)
        {
            return (grid[r+1][c] || grid[r][c+1] || grid[r][c-1]);
        }
        else
        {
            return 0;
        }
    }
    else if (r == gridSize - 1)
    {
        if (c == 0)
        {
            return (grid[r-1][c] && grid[r][c+1]);
        }
        else if (c == gridSize - 1)
        {
            return (grid[r-1][c] && grid[r][c-1]);
        }
        else if (c > 0 && c < gridSize - 1)
        {
          return (grid[r-1][c] || grid[r][c+1] || grid[r][c-1]);
        }
        else
        {
            return 0;
        }
    }
    else if (r > 0 && r < gridSize - 1)
    {
        if (c == 0)
        {
            return (grid[r-1][c] || grid[r+1][c] || grid[r][c+1]);
        }
        else if (c == gridSize - 1)
        {
           return (grid[r-1][c] || grid[r+1][c] || grid[r][c-1]);
        }
        else if (c > 0 && c < gridSize - 1)
        {
           return (grid[r][c-1] || grid[r][c+1] || grid[r+1][c] || grid[r-1][c]);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int validBox(char r, char c, char gridSize, char grid[][gridSize])
{
    return (connectedDot(r-1, c-1, gridSize, grid) && connectedDot(r-1, c+1, gridSize, grid) && connectedDot(r+1, c-1, gridSize, grid) && connectedDot(r+1, c+1, gridSize, grid));
}
int visited(char r, char c, int dfsStackIndex, int dfsStack[])
{
    int found = 0;
    int i = 0;
    while (!found && i < dfsStackIndex)
    {
        if (dfsStack[i] == c + r * 10) found = 1;
        i++;
    }
    return found;
}
int dfs(char r, char c, int *dfsStackIndex, int dfsStack[], char gridSize, char grid[][gridSize])
{
    int ri, l, u, d;
    ri = l = u = d = 1;
    printf("%d %d\n", r, c);
    if (!validBox(r, c, gridSize, grid)) return 0;
    dfsStack[*dfsStackIndex] = c + 10 * r; // Add box coordinate to stack
    *dfsStackIndex = *dfsStackIndex + 1;
    if (!grid[r-1][c] && !visited(r-2, c, *dfsStackIndex, dfsStack)) u = dfs(r-2, c, dfsStackIndex, dfsStack, gridSize, grid); // DFS up
    if (!grid[r+1][c] && !visited(r+2, c, *dfsStackIndex, dfsStack)) d = dfs(r+2, c, dfsStackIndex, dfsStack, gridSize, grid); // DFS down
    if (!grid[r][c+1] && !visited(r, c+2, *dfsStackIndex, dfsStack)) ri = dfs(r, c+2, dfsStackIndex, dfsStack, gridSize, grid); // DFS right
    if (!grid[r][c-1] && !visited(r, c-2, *dfsStackIndex, dfsStack)) l = dfs(r, c-2, dfsStackIndex, dfsStack, gridSize, grid); // DFS left
    return (ri && l && u && d);
}
int checkBox(char r1, char c1, char r2, char c2,char gridSize, char grid[][gridSize],char playerTurn ,char *remainingMoves) 
{
    int difficulty = (gridSize+1)/2;
    int dfsStackSize = difficulty * (difficulty - 1) * 2;
    int dfsStack[dfsStackSize];
    int dfsStackIndex = 0;
    emtpyArray(dfsStack, dfsStackSize, 0);
    char r=(char)fmin(r1, r2)+1;
    char c=(char)fmin(c1, c2)+1;
    int score = 0;
    int i;
    char tr, tc;
    // Row-wise move
    if (r1 == r2)
    {
        // Row is between borders
        if (r1 > 0 && r1 < gridSize-1)
        {
            // Check above
            if ((int)fabs(grid[r1-1][c-1])==1 && (int)fabs(grid[r1-1][c+1])==1 && (int)fabs(grid[r1-2][c])==1)
            {
                score++;
                dfsStack[0] = c + (r1-1) * 10;
                dfsStackIndex++;
                if (dfs(r1+1, c, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS below
                {
                    markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                    for (i = 0; i < dfsStackIndex; i++)
                    {
                        splitBoxCoordinates(dfsStack[i], &tr, &tc);
                        changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                        grid[tr][tc] = closedBox * playerTurn;
                    }
                    *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                    score+=dfsStackIndex-1;
                }
                else
                {
                    grid[r1-1][c] = closedBox * playerTurn;
                    changeBoxColor(r1-1, c, playerTurn, gridSize, grid);
                }
            dfsStackIndex = 0;
            }

            // Check below
            if ((int)fabs(grid[r1+1][c-1])==1 && (int)fabs(grid[r1+1][c+1])==1 && (int)fabs(grid[r1+2][c])==1)
            {
                score++;
                dfsStack[0] = c + (r1+1) * 10;
                dfsStackIndex++;
                if (dfs(r1-1, c, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS above
                {
                    markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                    for (i = 0; i < dfsStackIndex; i++)
                    {
                        splitBoxCoordinates(dfsStack[i], &tr, &tc);
                        changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                        grid[tr][tc] = closedBox * playerTurn;
                    }
                    *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                    score+=dfsStackIndex-1;
                }
                else
                {
                    grid[r1+1][c] = closedBox * playerTurn;
                    changeBoxColor(r1+1, c, playerTurn, gridSize, grid);
                }
            dfsStackIndex = 0;
            }
        }
        // Row is upmost
        else if (r1==0)
        {
            // Check below
            if ((int)fabs(grid[r1+1][c-1])==1 && (int)fabs(grid[r1+1][c+1])==1 && (int)fabs(grid[r1+2][c])==1)
            {
                score++;
                dfsStack[0] = c + (r1+1) * 10;
                dfsStackIndex++;
                if (dfs(r1-1, c, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS above
                {
                    markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                    for (i = 0; i < dfsStackIndex; i++)
                    {
                        splitBoxCoordinates(dfsStack[i], &tr, &tc);
                        changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                        grid[tr][tc] = closedBox * playerTurn;
                    }
                    *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                    score+=dfsStackIndex-1;
                }
                else
                {
                    grid[r1+1][c] = closedBox * playerTurn;
                    changeBoxColor(r1+1, c, playerTurn, gridSize, grid);
                }
            dfsStackIndex = 0;
            }
        }

        // Row is lowermost
        else
        {
            // Check above
            if ((int)fabs(grid[r1-1][c-1])==1 && (int)fabs(grid[r1-1][c+1])==1 && (int)fabs(grid[r1-2][c])==1)
            {
                score++;
                dfsStack[0] = c + (r1-1) * 10;
                dfsStackIndex++;
                if (dfs(r1+1, c, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS below
                {
                    markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                    for (i = 0; i < dfsStackIndex; i++)
                    {
                        splitBoxCoordinates(dfsStack[i], &tr, &tc);
                        changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                        grid[tr][tc] = closedBox * playerTurn;
                    }
                    *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                    score+=dfsStackIndex-1;
                }
                else
                {
                    grid[r1-1][c] = closedBox * playerTurn;
                    changeBoxColor(r1-1, c, playerTurn, gridSize, grid);
                }
            dfsStackIndex = 0;
            }
        }
    }
    // Column-wise move
    else
    {
        // Column is between borders
        if (c1 > 0 && c1 < gridSize-1)
        {
            if ((int)fabs(grid[r][c1-2])==1 && (int)fabs(grid[r-1][c1-1])==1 && (int)fabs(grid[r+1][c1-1])==1) // Left side
                {
                    score++;
                    dfsStack[0] = c1-1 + (r) * 10;
                    dfsStackIndex++;
                    if (dfs(r, c1+1, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS right
                    {
                        markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                        for (i = 0; i < dfsStackIndex; i++)
                        {
                            splitBoxCoordinates(dfsStack[i], &tr, &tc);
                            changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                            grid[tr][tc] = closedBox * playerTurn;
                        }
                        *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                        score+=dfsStackIndex-1;
                    }
                    else
                    {
                        grid[r][c1-1] = closedBox * playerTurn;
                        changeBoxColor(r, c1-1, playerTurn, gridSize, grid);
                    }
                dfsStackIndex = 0;
                }
            // Right side
            if ((int)fabs(grid[r][c1+2])==1 && (int)fabs(grid[r-1][c1+1])==1 && (int)fabs(grid[r+1][c1+1])==1)
                {
                    score++;
                    dfsStack[0] = c1+1 + (r) * 10;
                    dfsStackIndex++;
                    if (dfs(r, c1-1, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS left
                    {
                        markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                        for (i = 0; i < dfsStackIndex; i++)
                        {
                            splitBoxCoordinates(dfsStack[i], &tr, &tc);
                            changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                            grid[tr][tc] = closedBox * playerTurn;
                        }
                        *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                        score+=dfsStackIndex-1;
                    }
                    else
                    {
                        grid[r][c1+1] = closedBox * playerTurn;
                        changeBoxColor(r, c1+1, playerTurn, gridSize, grid);
                    }
                dfsStackIndex = 0;
                }
        }

        // Column is on the left border

        else if (c1 == 0)
        {
            // Right side
            if ((int)fabs(grid[r][c1+2])==1 && (int)fabs(grid[r-1][c1+1])==1 && (int)fabs(grid[r+1][c1+1])==1)
            {
                score++;
                dfsStack[0] = c1+1 + (r) * 10;
                dfsStackIndex++;
                if (dfs(r, c1-1, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS left
                {
                    markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                    for (i = 0; i < dfsStackIndex; i++)
                    {
                        splitBoxCoordinates(dfsStack[i], &tr, &tc);
                        changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                        grid[tr][tc] = closedBox * playerTurn;
                    }
                    *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                        score+=dfsStackIndex-1;
                }
                else
                {
                    grid[r][c1+1] = closedBox * playerTurn;
                    changeBoxColor(r, c1+1, playerTurn, gridSize, grid);
                }
            dfsStackIndex = 0;
            }
        }

        // Column is on the right border
        else
        {
            // Left side
             if ((int)fabs(grid[r][c1-2])==1 && (int)fabs(grid[r-1][c1-1])==1 && (int)fabs(grid[r+1][c1-1])==1)
                {
                    score++;
                    dfsStack[0] = c1-1 + (r) * 10;
                    dfsStackIndex++;
                    if (dfs(r, c1+1, &dfsStackIndex, dfsStack, gridSize, grid) && dfsStackIndex > 2) // DFS right
                    {
                        markBoxes(dfsStack, nonZero(dfsStack, dfsStackSize), gridSize, grid);
                        for (i = 0; i < dfsStackIndex; i++)
                        {
                            splitBoxCoordinates(dfsStack[i], &tr, &tc);
                            changeBoxColor(tr, tc, playerTurn, gridSize, grid);
                            grid[tr][tc] = closedBox * playerTurn;
                        }
                        *remainingMoves = *remainingMoves - dfsStackIndex + 2;
                            score+=dfsStackIndex-1;
                    }
                    else
                    {
                        grid[r][c1-1] = closedBox * playerTurn;
                        changeBoxColor(r, c1-1, playerTurn, gridSize, grid);
                    }
                dfsStackIndex = 0;
                }
        }
    }
    return score;
}
