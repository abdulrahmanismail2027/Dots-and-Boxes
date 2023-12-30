#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "datastruct.h"
#include "terminal.h"
void dumpGrid(char gridSize, char stackSize, char grid[][gridSize],char gridStack[][gridSize][stackSize],char stackIndex)
{
    int i, j;
    for (i = 0; i < gridSize; i++)
    {
        for (j = 0; j < gridSize; j++)
        {
            gridStack[i][j][stackIndex] = grid[i][j];
        }
    }
}
void popGrid(char gridSize, char stackSize, char grid[][gridSize],char gridStack[][gridSize][stackSize],char stackIndex)
{
    int i, j;
    for (i = 0; i < gridSize; i++)
    {
        for (j = 0; j < gridSize; j++)
        {
            grid[i][j] = gridStack[i][j][stackIndex];
        }
    }
}
void dumpGameInfo(gameInfo Game, gameInfo gameInfoStack[], char stackIndex)
{
    gameInfoStack[stackIndex] = Game;
}
void popGameInfo(gameInfo *Game, gameInfo gameInfoStack[], char stackIndex)
{
    *Game = gameInfoStack[stackIndex];
}
char saveGame(char gridSize, char grid[][gridSize], gameInfo gameInfo, Names playerNames, char saveCounter, int elapsedTime)
{
    int i, j;
    char stringSaveCounter;
    static char datetime[MAX_DATE_LENGTH];
    char arrayName[20]="array_";
    char instanceName[20] = "gameInstance_";
    FILE *file;
    if (saveCounter == 0) // Create folder then save
    {
        saveCounter++;
        // Get current time
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        // Format the date and time as a string
        strftime(datetime, sizeof(datetime), "%Y%m%d_%H%M%S", timeinfo);
        chdir("data\\saves");
        mkdir(datetime);
        file = fopen("saves", "a");
        fprintf(file, "%s\n", datetime); // Prints folder names in saves file
        fclose(file);
        chdir(datetime);
        file = fopen("saveInstances", "a"); // Prints how many save instances we have
        fprintf(file, "%d\n", saveCounter);
        fclose(file);
        stringSaveCounter = saveCounter + '0';
        strncat(arrayName, &stringSaveCounter, 1);
        strncat(instanceName, &stringSaveCounter, 1);
        // Print game state into files
        file = fopen("players", "w");
        if (!file)
            printf("Error: players names file not found!");
        fprintf(file, "%s\n%s", playerNames.player1Name, playerNames.player2Name);
        fclose(file);
        file = fopen(arrayName, "w");
        for (int i = 0; i < gridSize; i++)
        {
            for (j = 0; j < gridSize; j++)
            {
                fprintf(file,
                "%d ",
                grid[i][j]
                );
            }
            fprintf(file, "\n");
        }
        fclose(file);
        file = fopen(instanceName, "w");
        fprintf(file,"%d,%d,%d,%d,%d,%d,%d,%d\n",
        gameInfo.difficulty,
        gameInfo.gameMode,
        gameInfo.remainingMoves,
        gameInfo.playerTurn,
        gameInfo.player1Moves,
        gameInfo.player2Moves,
        gameInfo.player1Score,
        gameInfo.player2Score);
        fclose(file);
        file = fopen("time", "w");
        fprintf(file, "%d", elapsedTime);
        fclose(file);
        printf("Game state saved successfully, press ENTER to continue.\n");
        getchar();
        return saveCounter;
    }
    else if (saveCounter < 3)
    {
        saveCounter++;
        chdir("data\\saves"); chdir(datetime);
        file = fopen("saveInstances", "a"); // Prints how many save instances we have
        fprintf(file, "%d\n", saveCounter);
        fclose(file);
        stringSaveCounter = saveCounter + '0';
        strncat(arrayName, &stringSaveCounter, 1);
        chdir("data\\saves"); chdir(datetime);
        file = fopen(arrayName, "w");
        for (int i = 0; i < gridSize; i++)
        {
            for (j = 0; j < gridSize; j++)
            {
                fprintf(file,
                "%d ",
                grid[i][j]
                );
            }
            fprintf(file, "\n");
        }
        fclose(file);
        // Save gameInfo struct
        strncat(instanceName, &stringSaveCounter, 1);
        file = fopen(instanceName, "w");
        fprintf(file,"%d,%d,%d,%d,%d,%d,%d,%d\n",
        gameInfo.difficulty,
        gameInfo.gameMode,
        gameInfo.remainingMoves,
        gameInfo.playerTurn,
        gameInfo.player1Moves,
        gameInfo.player2Moves,
        gameInfo.player1Score,
        gameInfo.player2Score);
        fclose(file);
        file = fopen("time", "w");
        fprintf(file, "%d", elapsedTime);
        fclose(file);
        printf("Game state saved successfully, press ENTER to continue.\n");
        getchar();
        return saveCounter;
    }
    else
    {
        clearLine();
        printf("Error: you can't save more than 3 times in a single game, press ENTER to continue\n");
        getchar();
        return saveCounter;
    }
}
gameInfo loadStruct(int instanceChoice)
{
    FILE *file;
    gameInfoInt gameStruct;
    gameInfo gameStructChar;
    char stringInstanceChoice = (char)instanceChoice +'0';
    char structName[20] = "gameInstance_";
    strncat(structName, &stringInstanceChoice, 1);
    file = fopen(structName, "r");
    if (!file)
        printf("Error: info file not found!");
    fscanf(file,"%d,%d,%d,%d,%d,%d,%d,%d\n",
    &(gameStruct.difficulty),
    &(gameStruct.gameMode),
    &(gameStruct.remainingMoves),
    &(gameStruct.playerTurn),
    &(gameStruct.player1Moves),
    &(gameStruct.player2Moves),
    &(gameStruct.player1Score),
    &(gameStruct.player2Score));
    fclose(file);
    (gameStructChar.difficulty) = (char)(gameStruct.difficulty);
    (gameStructChar.gameMode) = (char)(gameStruct.gameMode);
    (gameStructChar.remainingMoves) = (char)(gameStruct.remainingMoves);
    (gameStructChar.playerTurn) = (char)(gameStruct.playerTurn);
    (gameStructChar.player1Moves) = (char)(gameStruct.player1Moves);
    (gameStructChar.player2Moves) = (char)(gameStruct.player2Moves);
    (gameStructChar.player1Score) = (char)(gameStruct.player1Score);
    (gameStructChar.player2Score) = (char)(gameStruct.player2Score);
    return gameStructChar;
}
void loadGrid(char gridSize, char grid[gridSize][gridSize],int instanceChoice)
{
    int i, j;
    FILE *file;
    int gridInt[gridSize][gridSize];
    char stringInstanceChoice = (char)instanceChoice +'0';
    char arrayName[20] = "array_";
    strncat(arrayName, &stringInstanceChoice, 1);
    file = fopen(arrayName, "r");
    if (!file)
    {
        printf("Error: grid file not found!");
    }
    int row = 0;
    while (!feof(file))
    {
    for (int i = 0; i < gridSize; i++)
    {
    if (fscanf(file, "%d", &gridInt[row][i]) == EOF)
        break;
    }
    row++;
    if (row == gridSize) break;
    }
    fclose(file);
    // Copy data from temporary integer grid to char grid
    for (i = 0; i < gridSize; i++)
    {
        for (j = 0; j < gridSize; j++)
        {
            grid[i][j]=(char)gridInt[i][j];
        }
    }
}
Names loadNames()
{
    FILE *file;
    Names playerNames;
    file = fopen("players", "r");
    if (!file)
        printf("Error: players names file not found!");
    fscanf(file, "%25[^\n]\n", playerNames.player1Name);
    fscanf(file, "%25[^\n]", playerNames.player2Name);
    fclose(file);
    return playerNames;
}
int loadTime()
{
    FILE *file;
    int refelapsedTime;
    file = fopen("time", "r");
    fscanf(file, "%d", &refelapsedTime);
    fclose(file);
    return refelapsedTime;
}