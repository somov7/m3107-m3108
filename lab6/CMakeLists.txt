cmake_minimum_required(VERSION 3.20)
project(lab6 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab6 main.c arc.c list.c argParse.c arc.h list.h argParse.h)
