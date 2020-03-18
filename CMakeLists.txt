# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.0)
project(AutoQuest)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp game/Game.cpp game/Game.hpp)

find_package(SDL2 REQUIRED)
add_subdirectory(external_includes/assimp)

# SDL
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
# ASSIMP
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
