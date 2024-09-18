cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

project(bnkchip8)

add_executable(main
    src/main.cpp

    src/chip8.h
    src/chip8.cpp

    src/renderer.h
    src/renderer.cpp

    external/glad/src/glad.c
    )

include_directories(
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    )

target_link_libraries(main
    ${CMAKE_SOURCE_DIR}/external/glfw/lib/glfw3.lib
    )

## Scripts
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo
    COMMAND ${CMAKE_SOURCE_DIR}/build/main.exe
    DEPENDS ${CMAKE_SOURCE_DIR}/build/main.exe
    USES_TERMINAL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
