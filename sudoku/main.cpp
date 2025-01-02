#include <iostream>
#include <fstream>
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
#ifdef DEBUG
                printf("Reading line #%u\n", i + 1);
#endif
                getline(fin, tempStr, '\n');
                uint8_t *row = NULL;
#ifdef DEBUG
                printf("Parsing line \"%s\"\n", tempStr.c_str());
#endif
                rtnVal = readLineSpaces(tempStr, *size, &row);
#ifdef DEBUG
                printf("Parsed line \"%s\"\n", tempStr.c_str());
#endif
                (*grid)[i] = row;
            }
        }

        fin.close();

        return rtnVal;
    }

    error_t readLineSpaces(const char *line, const size_t size, uint8_t **row)
    {
        return readLineSpaces(std::string(line), size, row);
    }

    error_t readLineSpaces(const std::string line, const size_t size, uint8_t **row)
    {
        error_t rtnVal{0, ""};
        *row = new uint8_t[size];

        size_t start = 0;
        size_t stop;
        for (int i = 0; i < size; i++)
        {
            stop = line.find(' ', start);
            #ifdef DEBUG
            printf("Start: %zu\n", start);
            printf("Stop: %zu\n", stop);
#endif
            const char *sub = line.substr(start, stop - start+1).c_str();
#ifdef DEBUG
            printf("Sub: %s\n", sub);
#endif
            (*row)[i] = (uint8_t)atoi(sub);
            start++;
        }

        return rtnVal;
    }

    void printGrid(uint8_t **grid, const size_t size)
    {
        if (grid == NULL)
        {
            printf("Grid is NULL\n");
        }
        else
        {
            printf("Grid is %p\n", grid);
            for (size_t i = 0; i < size; i++)
            {
                printf("grid[%zu] is %p\n", i, grid[i]);

                for (size_t j = 0; grid[i] != NULL && j < size; j++)
                {
                    printf((j > 0 ? " %u" : "%u"), grid[i][j]);
                }
                printf("\n");
            }
        }
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