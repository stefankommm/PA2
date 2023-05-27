cmake_minimum_required(VERSION 3.24)
project(Semestralka_PA2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(.)

add_executable(Semestralka_PA2
        Board.cpp
        Board.h
        Character.cpp
        Character.h
        Configuration.cpp
        Configuration.h
        Game.cpp
        Game.h
        IGhostMovement.cpp
        IGhostMovement.h
        InputHandler.cpp
        InputHandler.h
        main.cpp
        Modes/Mode.cpp
        Modes/Mode.h
        Renderer.cpp
        Renderer.h rang.hpp Modes/Playing.cpp)
