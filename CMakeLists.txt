cmake_minimum_required(VERSION 3.18)

project(bnkchip8)

add_executable(main
    src/main.cpp
)

## Scripts
add_custom_target(run-debug
    COMMAND ${CMAKE_SOURCE_DIR}/build/debug/main.exe
    DEPENDS ${CMAKE_SOURCE_DIR}/build/debug/main.exe
    USES_TERMINAL
)
