cmake_minimum_required(VERSION 3.20)
project(archiver C)

set(CMAKE_C_STANDARD 11)
#include_directories(Map)
add_executable(archiver main.c archiver.c archiver.h Map/bitsarray.c Map/prime.c Map/Set.c)
