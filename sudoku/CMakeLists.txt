cmake_minimum_required(VERSION 3.5.0)
project(sudoku VERSION 0.1.0.1 LANGUAGES C CXX)

add_executable(sudoku main.cpp version.cpp)

configure_file(version.h.in version.h)

target_include_directories(sudoku PUBLIC
                           "${PROJECT_BINARY_DIR}"
                          )