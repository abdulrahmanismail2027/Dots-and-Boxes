#include <stdio.h>
#include <math.h>
#include "terminal.h"
#include "move.h"
void readDigit(int startRange, int endRange,int *pvalue) // Reads an integer number within the specified range
{
    int inputCount; double buffer;
    inputCount = scanf("%lf", &buffer);
    clearBuffer();
    while (!(buffer > startRange && buffer < endRange) || inputCount!=1 || (modff(buffer, NULL)))
    {
        clearLine();
        printf("Invalid input\n");
        inputCount = scanf("%lf", &buffer);
        clearBuffer();
        clearLine();
    }
    *pvalue = (int)buffer;
}
void readDigitWithCancel(int startRange, int endRange,int *pvalue) // Reads an integer number within the specified range and -1 for cancel
{
    int inputCount; double buffer;
    inputCount = scanf("%lf", &buffer);
    clearBuffer();
    while (!(buffer > startRange && buffer < endRange || buffer == -1) || inputCount!=1 || (modff(buffer, NULL)))
    {
        clearLine();
        printf("Invalid input\n");
        inputCount = scanf("%lf", &buffer);
        clearBuffer();
        clearLine();
    }
    *pvalue = (int)buffer;
}
int readMove(char *r1, char *c1, char *r2, char *c2) // Reads numerically valid move and separates them into 4 coordinates
{
    int buffer;
    readDigitWithCancel(999, 10000, &buffer);
    if (buffer != -1)
    {
        splitMove(buffer, r1, c1, r2, c2);
        return buffer;
    }
    else
    {
        return buffer;
    }

}
