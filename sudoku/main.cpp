#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include "version.h"
#include "main.h"

using namespace sudoku;

#define DEBUG

int main(int argc, char *argv[])
{
    error_t error{
        0,
        ""};

    printVersion();
    if (argc < 2) // No input file specified
    {
        std::cout << "Invocation: " << std::endl
                  << argv[0] << " " << "<file>" << std::endl;
    }
    else
    {
        size_t size;
        uint8_t **grid = NULL;
        error = readFile(argv[1], &size, &grid);
#ifdef DEBUG
        printf("readFile(%s, %zu, %p) = %s\n", argv[1], size, grid, printError(error).c_str());
#endif
        printf("Starting Grid:\n");
        printGrid(grid, size);

        bool solved = false;
        error = solveRecursive(grid, size, &solved, 0, 0);

        printf("Ending Grid:\n");
        printGrid(grid, size);

        freeResources(grid, size);
    }

    if (error.code != 0)
    {
        printf("%d, %s", error.code, error.message.c_str());
    }

    return error.code;
}

namespace sudoku
{
    inline std::string printError(error_t error)
    {
        return error.code == 0 ? "0" : error.code + ", " + error.message;
    }

    // error_t readFile(const std::string file, size_t *size, uint8_t **grid)
    // {
    //     return readFile(file.c_str(), size, grid);
    // }

    error_t readFile(const char *file, size_t *size, uint8_t ***grid)
    {
        error_t rtnVal{0, ""};
        std::ifstream fin;

        fin.open(file);
        if (!fin.is_open())
        {
            rtnVal.code = -1;
            rtnVal.message = "Could not open file \"" + std::string(file) + '"';
        }
        else
        {
            unsigned int tempUint = 0;
            std::string tempStr;
            getline(fin, tempStr, '\n');

            sscanf_s(tempStr.c_str(), "%u", &tempUint);
            *size = tempUint;
            printf("Size: %u\n", tempUint);

            *grid = new uint8_t *[tempUint];

            for (unsigned int i = 0; i < tempUint && rtnVal.code == 0; i++)
            {
                // #ifdef DEBUG
                //                 printf("Reading line #%u\n", i + 1);
                // #endif
                getline(fin, tempStr, '\n');
                uint8_t *row = NULL;
                // #ifdef DEBUG
                //                 printf("Parsing line \"%s\"\n", tempStr.c_str());
                // #endif
                rtnVal = parseLine(tempStr, *size, &row);
                // #ifdef DEBUG
                //                 printf("Parsed line \"%s\"\n", tempStr.c_str());
                // #endif
                (*grid)[i] = row;
            }
        }

        fin.close();

        return rtnVal;
    }

    error_t parseLine(const char *line, const size_t size, uint8_t **row, const char delimiter)
    {
        return parseLine(std::string(line), size, row, delimiter);
    }

    error_t parseLine(const std::string line, const size_t size, uint8_t **row, const char delimiter)
    {
        error_t rtnVal{0, ""};
        *row = new uint8_t[size];

        std::stringstream ss(line);
        std::string temp;
        for (size_t i = 0; i < size; i++)
        {
            getline(ss, temp, ' ');
            (*row)[i] = (uint8_t)atoi(temp.c_str());
        }

        return rtnVal;
    }

    error_t solveRecursive(uint8_t **grid, const size_t size, bool *solved, const uint16_t col, const uint16_t row)
    {
        error_t rtnVal{0, ""};
        uint16_t nextCol = col + 1;
        uint16_t nextRow = row;
        if (nextCol >= size)
        {
            nextCol = 0;
            nextRow++;
        }

        if (row >= size)
        {
#ifdef DEBUG
            static size_t count = 1;
            printf("Mid-Run Grid #%zu:\n", count++);
            printGrid(grid, size);
#endif
            *solved = validateGrid(grid, size);
        }
        else if (grid[col][row] == 0)
        {
            for (uint8_t i = 1; i <= size && !*solved && rtnVal.code == 0; i++)
            {
                grid[col][row] = i;
                printf("grid[%u][%u] = %u\n", col, row, i);
                if (validateGrid(grid, size))
                {
                    rtnVal = solveRecursive(grid, size, solved, nextCol, nextRow);
                }
            }

            if (!*solved)
            {
                grid[col][row] = 0;
            }
        }
        else
        {
            rtnVal = solveRecursive(grid, size, solved, nextCol, nextRow);
        }

        return rtnVal;
    }

    bool validateGrid(uint8_t **grid, const size_t size)
    {
        bool valid = true;

        for (uint16_t i = 0; i < size && valid; i++)
        {
            valid &= validateRow(grid, size, i);
            valid &= validateCol(grid, size, i);
        }

        return valid;
    }

    bool validateRow(uint8_t **grid, const size_t size, const uint16_t row)
    {
        bool valid = true;

        for (uint16_t i = 0; i < size && valid; i++)
        {
            if (grid[i][row] == 0)
            {
                continue;
            }
            for (uint16_t j = i + 1; j < size && valid; j++)
            {
                if (grid[j][row] == 0)
                {
                    continue;
                }
                else if (grid[i][row] == grid[j][row])
                {
                    valid = false;
                }
            }
        }

        return valid;
    }

    bool validateCol(uint8_t **grid, const size_t size, const uint16_t col)
    {
        bool valid = true;

        for (uint16_t i = 0; i < size && valid; i++)
        {
            if (grid[col][i] == 0)
            {
                continue;
            }
            for (uint16_t j = i + 1; j < size && valid; j++)
            {
                if (grid[col][j] == 0)
                {
                    continue;
                }
                else if (grid[col][i] == grid[col][j])
                {
                    valid = false;
                }
            }
        }

        return valid;
    }

    void printGrid(uint8_t **grid, const size_t size)
    {
#ifdef DEBUG
        if (grid == NULL)
        {
            printf("Grid is NULL\n");
        }
        else
        {
            printf("Grid is %p\n", grid);
#endif
            for (size_t i = 0; i < size; i++)
            {
#ifdef DEBUG
                // printf("grid[%zu] is %p\n", i, grid[i]);
#endif
                for (size_t j = 0; grid[i] != NULL && j < size; j++)
                {
                    printf((j > 0 ? " %u" : "%u"), grid[i][j]);
                }
                printf("\n");
            }
#ifdef DEBUG
        }
#endif
    }

    void freeResources(uint8_t **grid, const size_t size)
    {
        if (grid != NULL)
        {
            for (size_t i = 0; i < size; i++)
            {
                delete[] grid[i];
            }
            delete[] grid;
        }
    }
}