#pragma once
void readDigit(int startRange, int endRange,int *pvalue); // Reads an integer number within the specified range
void readDigitWithCancel(int startRange, int endRange,int *pvalue); // Reads an integer number within the specified range and -1 for cancel
int readMove(char *r1, char *c1, char *r2, char *c2);