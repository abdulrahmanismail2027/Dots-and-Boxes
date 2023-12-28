#pragma once
#include "constants.h"
typedef struct
{
    char player1Name[MAX_NAME_LENGTH+1];
    char player2Name[MAX_NAME_LENGTH+1];
} Names;
typedef struct
{
    char difficulty;
    char gameMode;
    char remainingMoves; // Number of remaining moves
    char playerTurn; // Indicator to track whose turn it is at the moment
    char player1Moves; // Number of moves made by player 1
    char player2Moves; // Number of moves made by player 2
    char player1Score;
    char player2Score;
} gameInfo;
typedef struct
{
    int difficulty;
    int gameMode;
    int remainingMoves; // Number of remaining moves
    int playerTurn; // Indicator to track whose turn it is at the moment
    int player1Moves; // Number of moves made by player 1
    int player2Moves; // Number of moves made by player 2
    int player1Score;
    int player2Score;
} gameInfoInt;
typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
} playerHighscore;