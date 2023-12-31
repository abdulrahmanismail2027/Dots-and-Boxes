#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "constants.h"
#include "datastruct.h"
#include "boxes.h"
#include "terminal.h"
#include "managearrs.h"
#include "move.h"
#include "read.h"
#include "print.h"
#include "status.h"
#include "saveload.h"
#include "highscore.h"
int PvPGameFlow(gameInfo PvPGame,char size, char grid[size][size],Names playerNames,int refelapsedTime, char *executionDir)
{
    int errorFlag = 0;
    time_t startTime, endTime;
    int elapsedTime;
    elapsedTime += refelapsedTime;
    char saveCounter = 0;
    char r1, c1, r2, c2;
    char lastPlayerTurn = 1;
    char stackSize = PvPGame.remainingMoves + 1; // +1 Because the fully empty state
    char stackIndex = 0; // Points at the current state of the game
    char savedStackIndex = 0;
    int stack[stackSize]; int readMoveReturn;
    char indexToBeFilled = 0;
    char score;
    int inGameMenuChoice;
    char gridSize = PvPGame.difficulty * 2 - 1;
    char gridStack[gridSize][gridSize][stackSize];
    gameInfo gameInfoStack[stackSize];
    startTime = time(NULL);
    dumpGrid(gridSize, stackSize, grid, gridStack, 0);
    dumpGameInfo(PvPGame, gameInfoStack, 0);
    while (PvPGame.remainingMoves > 0)
    {
    continuePvPFlow:
    endTime = time(NULL); elapsedTime = difftime(endTime, startTime) + refelapsedTime;
    printGrid(gridSize, grid); // Clears screen
    printGameInfo(PvPGame, playerNames, elapsedTime);
    if (errorFlag) printf("Invalid input\n");
    errorFlag = 0;
    printf("Enter your move, enter -1 for in-game menu\n");
    readMoveReturn = readMove(&r1, &c1, &r2, &c2);
    if (readMoveReturn == -1) // Enter in-game menu
    {
        clearLine(); clearLine();
        printf("Enter the option of your choice\n1: Undo-Redo\t2: Save game\n3: Continue\t4: Quit game\n");
        readDigit(0, 5, &inGameMenuChoice);
        if (inGameMenuChoice == 1) // Undo - Redo
        {
            int undoredoChoice;
            clearLine();
            printf("Enter the option of your choice\n1: Undo\t2: Redo\n");
            readDigit(0, 3, &undoredoChoice);
            if (undoredoChoice == 1) // Undo
            {
                if (indexToBeFilled>0)
                {
                    indexToBeFilled--; stackIndex--;
                    popGrid(gridSize, stackSize, grid, gridStack, stackIndex);
                    popGameInfo(&PvPGame, gameInfoStack, stackIndex);
                    goto continuePvPFlow;
                }
                else
                {
                    clearLine();
                    printf("Error: no move to undo, press ENTER to continue\n");
                    getchar();
                    goto continuePvPFlow;
                }
            }
            else // Redo
            {
                // An Undo must be done for a redo to be possible
                if (stack[indexToBeFilled]==1) // A redo is possible
                {
                    indexToBeFilled++; stackIndex++;
                    popGrid(gridSize, stackSize, grid, gridStack, stackIndex);
                    popGameInfo(&PvPGame, gameInfoStack, stackIndex);
                    goto continuePvPFlow;
                }
                else
                {
                    clearLine();
                    printf("Error: no move to redo, press ENTER to continue\n");
                    getchar();
                    goto continuePvPFlow;
                }

            }
        }
        if (inGameMenuChoice == 2) // Save game
        {
            chdir(executionDir);
            saveCounter = saveGame(gridSize, grid, PvPGame, playerNames, saveCounter, elapsedTime);
            goto continuePvPFlow;
        }
        if (inGameMenuChoice == 3)
        {
            goto continuePvPFlow;
        }
        if (inGameMenuChoice == 4) // Go back to main menu
        {
            return 1;
        }
    }
    mapMove(&r1, &c1, &r2, &c2);
    if(!validMove(demap(r1), demap(c1), demap(r2), demap(c2), gridSize) || !validCell(r1, c1, r2, c2, gridSize, grid))
    {
        errorFlag = 1; goto continuePvPFlow;
    }
    // Handles stack array
    if (lastPlayerTurn == PvPGame.playerTurn)
    {
        stack[indexToBeFilled]=1;
        indexToBeFilled++;
        emtpyArray(stack, stackSize, indexToBeFilled);
    }
    else
    {
        emtpyArray(stack, stackSize, 0); indexToBeFilled = 0;
        stack[indexToBeFilled]=1;
        indexToBeFilled++;
    }
    score = checkBox(r1, c1, r2, c2, gridSize, grid, PvPGame.playerTurn, &PvPGame.remainingMoves);
    realizeMove(r1, c1, r2, c2, gridSize, grid, PvPGame.playerTurn);
    incrementScore(score, &PvPGame.player1Score, &PvPGame.player2Score, PvPGame.playerTurn);
    incrementMove(&PvPGame.player1Moves, &PvPGame.player2Moves, PvPGame.playerTurn, score);
    lastPlayerTurn = PvPGame.playerTurn;
    determineTurn(&PvPGame.playerTurn, score);
    PvPGame.remainingMoves--;
    // Dump
    if (stack)
    stackIndex++;
    savedStackIndex=stackIndex;
    dumpGrid(gridSize, stackSize, grid, gridStack, stackIndex);
    dumpGameInfo(PvPGame, gameInfoStack, stackIndex);
    }

    printGrid(gridSize, grid); // Clears screen
    printGameInfo(PvPGame, playerNames, elapsedTime);
    if (PvPGame.player1Score > PvPGame.player2Score)
    {
        printf(ANSI_COLOR_GREEN"Congratulations %s, your are the winner!\n"ANSI_COLOR_RESET, playerNames.player1Name);
        handleRank(playerNames.player1Name, PvPGame.player1Score, executionDir);
    }
    else if (PvPGame.player1Score < PvPGame.player2Score)
    {
        printf(ANSI_COLOR_GREEN"Congratulations %s, your are the winner!\n"ANSI_COLOR_RESET, playerNames.player2Name);
        handleRank(playerNames.player2Name, PvPGame.player2Score, executionDir);
    }
    else
    {
        printf(ANSI_COLOR_CYAN"No player won, it's a tie!\n"ANSI_COLOR_RESET);
        printHighscores(executionDir);
    }
    int choice;
    printf("Enter an option\n1: Go back to main menu\t2: Quit game\n");
    readDigit(0, 3, &choice);
    return choice;
}
int PvCGameFlow(gameInfo PvCGame,char size, char grid[size][size],Names playerNames,int refelapsedTime, char *executionDir)
{
    int errorFlag = 0;
    time_t startTime, endTime;
    int elapsedTime;
    elapsedTime += refelapsedTime;
    char saveCounter = 0;
    char r1, c1, r2, c2;
    char lastPlayerTurn = 1;
    char stackSize = PvCGame.remainingMoves + 1; // +1 Because the fully empty state
    char stackIndex = 0; // Points at the current state of the game
    int stack[stackSize]; int readMoveReturn;
    char indexToBeFilled = 0;
    char score;
    int  inGameMenuChoice;
    char gridSize = PvCGame.difficulty * 2 - 1;
    char gridStack[gridSize][gridSize][stackSize];
    gameInfo gameInfoStack[stackSize];
    startTime = time(NULL);
    dumpGrid(gridSize, stackSize, grid, gridStack, 0);
    dumpGameInfo(PvCGame, gameInfoStack, 0);
    while (PvCGame.remainingMoves > 0)
    {
    continuePvCFlow:
    endTime = time(NULL); elapsedTime = difftime(endTime, startTime) + refelapsedTime;
    printGrid(gridSize, grid); // Clears screen
    printGameInfo(PvCGame, playerNames, elapsedTime);
    if (PvCGame.playerTurn == 1) // Player turn
    {
        if (errorFlag) printf("Invalid input\n"); errorFlag = 0;
        printf("Enter your move, enter -1 for in-game menu\n");
        readMoveReturn = readMove(&r1, &c1, &r2, &c2);
        if (readMoveReturn == -1) // Enter in-game menu
        {
            clearLine(); clearLine();
            printf("Enter the option of your choice\n1: Undo-Redo\t2: Save game\n3: Continue\t4: Quit game\n");
            readDigit(0, 5, &inGameMenuChoice);
            if (inGameMenuChoice == 1) // Undo - Redo
            {
                int undoredoChoice;
                clearLine();
                printf("Enter the option of your choice\n1: Undo\t2: Redo\n");
                readDigit(0, 3, &undoredoChoice);
                if (undoredoChoice == 1) // Undo
                {
                    if (indexToBeFilled>0 && lastPlayerTurn == PvCGame.playerTurn)
                    {
                        indexToBeFilled--; stackIndex--;
                        popGrid(gridSize, stackSize, grid, gridStack, stackIndex);
                        popGameInfo(&PvCGame, gameInfoStack, stackIndex);
                        goto continuePvCFlow;
                    }
                    else
                    {
                        clearLine();
                        printf("Error: no move to undo, press ENTER to continue\n");
                        getchar();
                        goto continuePvCFlow;
                    }
                }
                else // Redo
                {
                    // An Undo must be done for a redo to be possible
                    if (stack[indexToBeFilled]==1) // A redo is possible
                    {
                        indexToBeFilled++; stackIndex++;
                        popGrid(gridSize, stackSize, grid, gridStack, stackIndex);
                        popGameInfo(&PvCGame, gameInfoStack, stackIndex);
                        goto continuePvCFlow;
                    }
                    else
                    {
                        clearLine();
                        printf("Error: no move to redo, press ENTER to continue\n");
                        getchar();
                        goto continuePvCFlow;
                    }

                }
            }
            if (inGameMenuChoice == 2) // Save game
            {
                chdir(executionDir);
                saveCounter = saveGame(gridSize, grid, PvCGame, playerNames, saveCounter, elapsedTime);
                goto continuePvCFlow;
            }
            if (inGameMenuChoice == 3)
            {
                goto continuePvCFlow;
            }
            if (inGameMenuChoice == 4) // Go back to main menu
            {
                return 1;
            }
        }
        mapMove(&r1, &c1, &r2, &c2);
        if(!validMove(demap(r1), demap(c1), demap(r2), demap(c2), gridSize) || !validCell(r1, c1, r2, c2, gridSize, grid))
        {
            errorFlag = 1; goto continuePvCFlow;
        }
    }
    else // Computer turn
    {
        generateMove(&r1, &c1, &r2, &c2);
        mapMove(&r1, &c1, &r2, &c2);
        while(!validMove(demap(r1), demap(c1), demap(r2), demap(c2), gridSize) || !validCell(r1, c1, r2, c2, gridSize, grid))
        {
            generateMove(&r1, &c1, &r2, &c2);
            mapMove(&r1, &c1, &r2, &c2);
        }
    }
    // Handles stack array
    if (lastPlayerTurn == PvCGame.playerTurn)
    {
        stack[indexToBeFilled]=1;
        indexToBeFilled++;
        emtpyArray(stack, stackSize, indexToBeFilled);
    }
    else
    {
        emtpyArray(stack, stackSize, 0); indexToBeFilled = 0;
        stack[indexToBeFilled]=1;
        indexToBeFilled++;
    }
    score = checkBox(r1, c1, r2, c2, gridSize, grid, PvCGame.playerTurn, &PvCGame.remainingMoves);
    realizeMove(r1, c1, r2, c2, gridSize, grid, PvCGame.playerTurn);
    incrementScore(score, &PvCGame.player1Score, &PvCGame.player2Score, PvCGame.playerTurn);
    incrementMove(&PvCGame.player1Moves, &PvCGame.player2Moves, PvCGame.playerTurn, score);
    lastPlayerTurn = PvCGame.playerTurn;
    determineTurn(&PvCGame.playerTurn, score);
    PvCGame.remainingMoves--;
    // Dump
    stackIndex++;
    dumpGrid(gridSize, stackSize, grid, gridStack, stackIndex);
    dumpGameInfo(PvCGame, gameInfoStack, stackIndex);
    }   // Game ends
    printGrid(gridSize, grid); // Clears screen
    printGameInfo(PvCGame, playerNames, elapsedTime);
    if (PvCGame.player1Score > PvCGame.player2Score)
    {
        printf(ANSI_COLOR_GREEN"Congratulations %s, you won!\n"ANSI_COLOR_RESET, playerNames.player1Name);
        handleRank(playerNames.player1Name, PvCGame.player1Score, executionDir);
    }
    else if (PvCGame.player1Score < PvCGame.player2Score)
    {
        printf(ANSI_COLOR_RED"Sorry %s, you lost!\n"ANSI_COLOR_RESET, playerNames.player1Name);
        printHighscores(executionDir);
    }
    else
    {
        printf(ANSI_COLOR_CYAN"No one won, it's a tie!\n"ANSI_COLOR_RESET);
        printHighscores(executionDir);
    }
    int choice;
    printf("Enter an option\n1: Go back to main menu\t2: Quit game\n");
    readDigit(0, 3, &choice);
    return choice;
}
void main()
{
    mkdir("data"); mkdir("data\\saves"); // Creates the required folders if they don't exist already
    FILE *file;
    file = fopen("data\\highscores", "r");
    if (!file)                                  // Creates highscore file in case it doesn't exist
        file = fopen("data\\highscores", "w");
    fclose(file);
    char executionDir[MAX_PATH_LENGTH]; getcwd(executionDir, sizeof(executionDir));
    int buffer, result;
    mainMenuQuery:
    clearScreen();
    int mainMenuChoice;
    printf("Enter the option of your choice\n1: Start game\t2: Load game\n3: Highscores\t4: Quit game\n");
    readDigit(0, 5, &mainMenuChoice);
    if (mainMenuChoice==1) // Start game
        {
            // Read difficulty
            char difficultyChoice, difficulty, gameMode;
            int gameModeChoice;
            difficultyQuery:
            clearScreen();
            printf("Enter the difficulty of your choice\n1: Beginner\t2: Expert\n3: Go back to main menu\n");
            readDigit(0, 4, &buffer);
            difficultyChoice=(char)buffer;
            if (difficultyChoice==1)
            {
                difficulty=3;
            }

            if (difficultyChoice==2)
            {
                difficulty=6;
            }

            if (difficultyChoice==3)
            {
                goto mainMenuQuery;
            }

            // Read game mode
            clearScreen();
            printf("Enter the game mode of your choice\n1: PvP\t2: PvC\n3: Change difficulty choice\t4: Go back to main menu\n");
            readDigit(0, 5, &gameModeChoice);
            if (gameModeChoice==1) // PvP
            {
                gameMode = 1;
                // Get players' names
                Names playerNames;
                clearScreen();
                printf("Please enter the name of the first player (20 characters max):\n");
                fgets(playerNames.player1Name, MAX_NAME_LENGTH, stdin);
                clearScreen();
                printf("Please enter the name of the second player (20 characters max):\n");
                fgets(playerNames.player2Name, MAX_NAME_LENGTH, stdin);
                removeNewLine(playerNames.player1Name);
                removeNewLine(playerNames.player2Name);
                gameInfo freshPvPGame = {difficulty, gameMode, (difficulty*(difficulty-1)*2), 1, 0, 0, 0, 0};
                char gridSize = difficulty*2-1;
                char grid[gridSize][gridSize];
                initializeGrid(gridSize, grid);
                result = PvPGameFlow(freshPvPGame, gridSize, grid, playerNames, 0, executionDir);
                if (result == 1)
                    goto mainMenuQuery;
                else
                    return;
            }
            if (gameModeChoice==2) // PvC
            {
                gameMode = 2;
                Names playerNames;
                clearScreen();
                printf("Please enter your name (20 characters max):\n");
                fgets(playerNames.player1Name, MAX_NAME_LENGTH, stdin);
                removeNewLine(playerNames.player1Name);
                strcpy(playerNames.player2Name, "Computer");
                gameInfo freshPvCGame = {difficulty, gameMode, (difficulty*(difficulty-1)*2), 1, 0, 0, 0, 0};
                char gridSize = difficulty*2-1;
                char grid[gridSize][gridSize];
                initializeGrid(gridSize, grid);
                result = PvCGameFlow(freshPvCGame, gridSize, grid, playerNames, 0, executionDir);
                if (result == 1)
                    goto mainMenuQuery;
                else
                    return;
            }
            if (gameModeChoice==3)
            {
                goto difficultyQuery;
            }
            if (gameModeChoice==4)
            {
                goto mainMenuQuery;
            }
        } // Start game
    if (mainMenuChoice==2) // Load saved game
    {
        clearLine();
        char line[MAX_DATE_LENGTH];
        FILE *file;
        chdir(executionDir);
        // Read the required game to load
        file = fopen("data\\saves\\saves", "r");
        if (!file)
        {
            printf("No saved games to load!, press ENTER to go back to the main menu\n");
            getchar();
            goto mainMenuQuery;
        }
        int i = 1; int j = 0;
        while (fscanf(file, "%s", line) != EOF)
        {
            printf("%d -- %s\n", i, line);
            i++;
        }
        fclose(file);
        int folderChoice, instanceChoice;
        readDigit(0, i, &folderChoice); // i = number of saves + 1
        file = fopen("data\\saves\\saves", "r");
        while (j!=folderChoice && fscanf(file, "%s", line) != EOF)
        {
            j++;
        }
        fclose(file);
        chdir("data\\saves"); chdir(line); // Now we are in saved game folder
        file = fopen("saveInstances", "r");
        i = 1;
        while (fscanf(file, "%s[^\n]", line) != EOF)
        {
            printf("Save %d\n", i);
            i++;
        }
        fclose(file);
        printf("Enter the save instance you want to load\n");
        readDigit(0, i, &instanceChoice);
        gameInfo gameStruct;
        Names playerNames;
        playerNames = loadNames();
        gameStruct = loadStruct(instanceChoice);
        char gridSize = gameStruct.difficulty*2-1;
        char grid[gridSize][gridSize];
        loadGrid(gridSize, grid, instanceChoice);
        int refelapsedTime = loadTime();
        if (gameStruct.gameMode==1) // Load PvP game
        {
            result = PvPGameFlow(gameStruct, gridSize, grid, playerNames, refelapsedTime, executionDir);
            if (result == 1)
                goto mainMenuQuery;
            else
                return;
        }
        else // Load PvC game
        {
            result = PvCGameFlow(gameStruct, gridSize, grid, playerNames, refelapsedTime, executionDir);
            if (result == 1)
                goto mainMenuQuery;
            else
                return;
        }
    }
    if (mainMenuChoice==3) // Print top 10
    {
        printHighscores(executionDir);
        printf("Press ENTER to go back to the main menu\n");
        getchar();
        goto mainMenuQuery;
    }
    if (mainMenuChoice==4) // Exit
    {
        return;
    }
}
