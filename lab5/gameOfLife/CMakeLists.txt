cmake_minimum_required(VERSION 3.20)
project(gameOfLife C)

set(CMAKE_C_STANDARD 11)

add_executable(gameOfLife main.c game.c game.h bmp.c bmp.h)
