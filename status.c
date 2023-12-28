void determineTurn(char *playerTurn, char score)
{
    if (score==0) // Player didn't score
    {
        *playerTurn *= -1;
        return;
    }
    else // Player scored
    {
        return;
    }
}
void incrementScore(char score, char *player1Score, char* player2Score, char playerTurn)
{
    if (playerTurn == 1)
    {
        *player1Score+=score;
        return;
    }
    else
    {
        *player2Score+=score;
        return;
    }

}
void incrementMove(char *player1Moves, char *player2Moves, char playerTurn,char score)
{
    if (playerTurn == 1)
    {
        *player1Moves +=1;
        if (score > 1) *player1Moves +=score - 2;
    }
    else
    {
        *player2Moves +=1;
        if (score > 1) *player2Moves +=score - 2;
    }
}
