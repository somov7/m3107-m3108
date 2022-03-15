cmake_minimum_required(VERSION 3.20)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab5 main.c argparse/argparse.h argparse/argparse.c game/game.c game/game.h helpers/helpers.c helpers/helpers.h)