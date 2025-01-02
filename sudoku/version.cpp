#include "version.h"
#include <stdio.h>

void printVersion(bool appendLine)
{
    printf("Sudoku v%d.%d.%d.%d", SUDOKU_VERSION_MAJOR, SUDOKU_VERSION_MINOR, SUDOKU_VERSION_PATCH, SUDOKU_VERSION_TWEAK);
    if(appendLine)
    {
        printf("\n");
    }
}