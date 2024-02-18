cmake_minimum_required(VERSION 3.28)
project(gbemu C)

set(CMAKE_C_STANDARD 11)

add_executable(gbemu gb.c
            cpu.c
            cpu.h
        gb.h)

find_package(SDL2 REQUIRED)

if(GCC)
    target_compile_options(-Wall -Wextra -Wpedantic)
endif()