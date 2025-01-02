#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>
#include <string>

typedef struct
{
    int code;
    std::string message;
} error_t;

namespace sudoku
{
    std::string printError(error_t error);

    error_t readFile(const char *file, size_t *size, uint8_t ***grid);

    error_t parseLine(const char *line, const size_t size, uint8_t **row, const char delimiter = ' ');

    error_t parseLine(const std::string line, const size_t size, uint8_t **row, const char delimiter = ' ');

    error_t solveLoop(uint8_t **grid, const size_t size, bool *solved);

    error_t solveRecursive(uint8_t **grid, const size_t size, bool *solved, const uint16_t col = 0, const uint16_t row = 0);

    bool validateGrid(uint8_t **grid, const size_t size);

    bool validateRow(uint8_t **grid, const size_t size, const uint16_t fow);
    
    bool validateCol(uint8_t **grid, const size_t size, const uint16_t col);

    void printGrid(uint8_t **grid, const size_t size);

    void freeResources(uint8_t **grid, const size_t size);
}

#endif
