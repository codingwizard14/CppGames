cmake_minimum_required(VERSION 3.5.0)
project(CppGames VERSION 0.1.0 LANGUAGES C CXX)

add_executable(helloWorld helloWorld/src/main.cpp)

include(CTest)
enable_testing()

