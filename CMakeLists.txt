cmake_minimum_required(VERSION 4.0)
project(console_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(console_game main.cpp
        Game.cpp
        Game.h)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(console_game ${CURSES_LIBRARIES})