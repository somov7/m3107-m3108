cmake_minimum_required(VERSION 3.20)
project(wordcount C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(wordcount
        main.c utils.h)
