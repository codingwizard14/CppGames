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

    error_t readLineSpaces(const char *line, const size_t size, uint8_t **row);

    error_t readLineSpaces(const std::string line, const size_t size, uint8_t **row);

    void printGrid(uint8_t **grid, const size_t size);

    void freeResources(uint8_t **grid, const size_t size);
}

#endif
