cmake_minimum_required(VERSION 3.28)
project(gbemu C)

set(CMAKE_C_STANDARD 11)

add_executable(gbemu main.c)

find_package(SDL2 REQUIRED)


