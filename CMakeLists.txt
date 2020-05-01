# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.0)
project(AutoQuest)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp game/Game.cpp game/Game.hpp game/vk/Vulkanize.cpp game/vk/Vulkanize.hpp)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
add_subdirectory(external_includes/assimp)
# Vulkan
target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
# SDL
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
# ASSIMP
# target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

# GLSLC command to compile shaders to SPIR-V
find_program(GLSLC glslc)
set(shader_path ${CMAKE_HOME_DIRECTORY}/assets/shaders/)
file(GLOB shaders RELATIVE ${CMAKE_SOURCE_DIR} "${shader_path}*.vert" "${shader_path}*.frag")
foreach(shader ${shaders})
    set(input_glsl "${CMAKE_HOME_DIRECTORY}/${shader}")
    set(output_spv "${input_glsl}.spv")
    execute_process(COMMAND "glslc" "${input_glsl}" "-o" "${output_spv}")
endforeach()