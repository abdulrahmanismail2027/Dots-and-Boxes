#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
void removeNewLine(char *input)
{
int ln = strlen(input) - 1;
if (input[ln] == '\n')
    input[ln] = '\0';
}
void halt()
{
    int h;
    scanf("%d", &h);
}
void clearLine()
{
    printf("\033[A\033[2K");
}
void clearScreen()
{
    system("clear || cls");
}
void clearBuffer()
{
    while (getchar() != '\n');
}