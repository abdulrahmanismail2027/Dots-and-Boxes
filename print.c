#include <stdio.h>
#include "datastruct.h"
#include "constants.h"
#include "terminal.h"
void initializeGrid(char gridSize, char grid[][gridSize])
{
    int i, j;
    for (i=0; i<gridSize; i++)
    {
        for (j=0; j<gridSize; j++)
        if (i%2==0 && j%2==0)
        {
            grid[i][j]=dot;
        } else if (i%2!=0 && j%2!=0)
        {
            grid[i][j]=blank;
        }
        else
        {
            grid[i][j]=0;
        }


    }
}
void printGrid(char gridSize, char grid[][gridSize])
{
    clearScreen();
    int i, j;
    // Print column indices
    printf("     ");
    for (i=1; i<=(gridSize+1)/2; i++)
    {
        printf("%d    ", i);
    }
    printf("\n\n");
    for (i=0; i<gridSize; i++)
    {
        // Print row indices
        if (i%2==0)
        {
            printf("%d    ", i/2+1);
        }
        else
        {
            printf("     ");
        }

        for (j=0; j<gridSize; j++)
        {
            // Even rows (horizontal lines case)
            if (i%2==0)
            {
                if (grid[i][j]==1)
                    printf(ANSI_COLOR_RED"----"ANSI_COLOR_RESET);
                if (grid[i][j]==-1)
                    printf(ANSI_COLOR_BLUE"----"ANSI_COLOR_RESET);
                if (grid[i][j]==0 || grid[i][j]==chain)
                    printf("    ");
            }

            // Odd rows (vertical lines case)
            else
            {
                if (grid[i][j]==1)
                    printf(ANSI_COLOR_RED"|"ANSI_COLOR_RESET);
                if (grid[i][j]==-1)
                    printf(ANSI_COLOR_BLUE"|"ANSI_COLOR_RESET);
                if (grid[i][j]==blank)
                    printf("    ");
                if (grid[i][j]==0 || grid[i][j]==chain)
                    printf(" ");
                if (grid[i][j]==player1Box)
                    printf(ANSI_COLOR_RED" P1 "ANSI_COLOR_RESET);
                if (grid[i][j]==player2Box)
                    printf(ANSI_COLOR_BLUE" P2 "ANSI_COLOR_RESET);
            }
            // Prints dots
            if (grid[i][j]==dot)
                printf("o");
        }
        printf("\n");
    }
}
void printGameInfo(gameInfo game,Names playerNames, int time)
{
    printf("\n");
    if (game.playerTurn==1)
        printf(ANSI_COLOR_RED"%s's Turn"ANSI_COLOR_RESET, playerNames.player1Name);
    else
        printf(ANSI_COLOR_BLUE"%s's Turn"ANSI_COLOR_RESET, playerNames.player2Name);
    printf("\n");
    printf(ANSI_COLOR_RED"%s's Moves: %d"ANSI_COLOR_RESET ANSI_COLOR_BLUE"\t\t%s's Moves: %d"ANSI_COLOR_RESET,playerNames.player1Name, game.player1Moves, playerNames.player2Name, game.player2Moves);
    printf("\n");
    printf(ANSI_COLOR_RED"%s's Score: %d"ANSI_COLOR_RESET ANSI_COLOR_BLUE"\t\t%s's Score: %d"ANSI_COLOR_RESET,playerNames.player1Name, game.player1Score , playerNames.player2Name,game.player2Score);
    printf("\n");
    printf(ANSI_COLOR_YELLOW"Remaining Moves: %d\tTime Elapsed: %d Minutes, %d Seconds"ANSI_COLOR_RESET, game.remainingMoves, time/60, time%60);
    printf("\n");
}
void printMatrix(char gridSize, char grid[][gridSize])
{
    int i, j;
    for (i=0; i<gridSize; i++)
    {
        for (j=0; j<gridSize; j++)
        {
            printf("%2d ", grid[i][j]);
        }
        printf("\n");
    }
}
void printArray(int arr[], char size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
}