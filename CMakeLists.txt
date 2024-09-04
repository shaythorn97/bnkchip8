cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(bnkchip8)

add_executable(main
    src/main.cpp

    src/chip8.h
    src/chip8.cpp
)

## Scripts
add_custom_target(run-debug
    COMMAND ${CMAKE_SOURCE_DIR}/build/debug/main.exe
    DEPENDS ${CMAKE_SOURCE_DIR}/build/debug/main.exe
    USES_TERMINAL
)
