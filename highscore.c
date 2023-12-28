#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "datastruct.h"
int readHighscores(playerHighscore players[], char *executionDir)
{
    chdir(executionDir);
    FILE *file = fopen("data\\highscores", "r");
    if (!file)
        printf("Can't open file\n");
    int records = 0;
    int read = 0;
    do
    {
        read = fscanf(file, "%25[^,],%d\n",
                            players[records].name,
                            &players[records].score);
        if (read == 2)
            records++;
    } while (!feof(file));
    fclose(file);
    return records;
}
void sortHighscores(playerHighscore players[],int records)
{
    int i, j;
    playerHighscore temp;
    for (i = 0; i < records - 1; i++)
    {
        for (j = i+1; j < records; j++)
        {
            if (players[i].score  < players[j].score) // Then switch
             {
                temp = players[i];
                players[i] = players[j];
                players[j] = temp;
             }
        }
    }
}
int searchHighscores(char *name, playerHighscore players[],int records)
{
    int found = 0; int i = 0;
    while (!found && i < records)
        {
            if (strcasecmp(players[i].name, name) == 0)
                found = 1;
            i++;
        }
    if (found)
        return i-1;
    else
        return -1;
}
void writeHighscores(int records, playerHighscore players[], char *executionDir)
{
    FILE *file;
    int i;
    chdir(executionDir);
    file = fopen("data\\highscores", "w");
    if (!file)
        printf("Error openning file on write mode!");
    for (i = 0; i < records; i++)
    {
        fprintf(file, "%s,%d\n",
                        players[i].name,
                        players[i].score);
    }
    fclose(file);
}
void printHighscores(char *executionDir)
{
    chdir(executionDir);
    int i;
    playerHighscore players[MAX_RECORDS];
    FILE* file = fopen("data\\highscores", "r");
    if (!file)
        printf("Can't open file\n");
    int records = 0;
    int read = 0;
    do
    {
        read = fscanf(file, "%25[^,],%d\n",
                            players[records].name,
                            &players[records].score);
        if (read == 2)
            records++;
        else
        {
            printf("Highscore list is empty!\n");
            return;
        }


    } while (!feof(file) && records < 10);
    fclose(file);
    // Print
    printf(ANSI_COLOR_CYAN"Top players:\n"ANSI_COLOR_RESET);
    for (i = 0; i < records; i++)
    {
        printf("Name: %-15s\tHighscore: %d\n", players[i].name, players[i].score);
    }
}
void handleRank(char *name, int score, char *executionDir)
{
    playerHighscore players[MAX_RECORDS];
    int records = readHighscores(players, executionDir);
    if (!records)
    {
        // Append
        strcpy(players[0].name, name);
        players[0].score=score;
        records++;
        printf("Your score is: %d, your rank is: 1\n", score);
    }
    else
    {
        sortHighscores(players, records);
        int found = searchHighscores(name, players, records);
        if (found == -1) // Not found
        {
            if (records < MAX_RECORDS)
            {
                strcpy(players[records].name, name);
                players[records].score=score;
                records++;
                sortHighscores(players, records);
                found = searchHighscores(name, players, records);
                printf("Your score is: %d, your rank is: %d\n", score, found+1);
            }
            else
            {
                if (score >= players[records-1].score) // Then replace
                {
                    strcpy(players[records-1].name, name);
                    players[records-1].score=score;
                    sortHighscores(players, records);
                    found = searchHighscores(name, players, records);
                    printf("Your score is: %d, your rank is: %d\n", score, found+1);
                }
            }
        }
        else // Found
        {
            if (players[found].score < score) // Then update score
                players[found].score = score;
            sortHighscores(players, records);
            found = searchHighscores(name, players, records);
            printf("Your score is: %d, your rank is: %d\n", score, found+1);
        }
    }
    writeHighscores(records, players, executionDir);
    printHighscores(executionDir);
}
