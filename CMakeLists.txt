cmake_minimum_required(VERSION 3.28)
project(gbemu C)

set(CMAKE_C_STANDARD 17)

add_executable(gbemu gb.c
        gb.h
        cpu.c
        cpu.h)

find_package(SDL2 REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c17 -pipe -g -Wall -Wextra -Wpedantic")