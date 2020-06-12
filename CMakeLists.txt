# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.0)
project(AutoQuest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS true)
set(CMAKE_MODULE_PATH cmake_modules)

find_package(SFML 2.5 COMPONENTS audio graphics network system window REQUIRED)
find_package(Freetype REQUIRED)

include_directories(external_includes/glm/)
add_subdirectory(external_includes/glad/)
add_subdirectory(external_includes/entt/)

# add game directory
include_directories(game/)

add_executable(${PROJECT_NAME} main.cpp game/Game.cpp game/Game.hpp
        game/systems/InputSystem.cpp game/systems/InputSystem.hpp
        game/systems/ShaderSystem.cpp game/systems/ShaderSystem.hpp
        game/systems/SettingsSystem.cpp game/systems/SettingsSystem.hpp
        game/systems/SceneSystem.cpp game/systems/SceneSystem.hpp game/components/Settings.hpp
        game/systems/RenderSystem.cpp game/systems/RenderSystem.hpp
        game/resource/RenderTarget.cpp game/resource/RenderTarget.hpp
        game/components/Renderable.hpp
        game/Shapes.hpp
        game/components/Clickable.hpp
        game/resource/Shader.cpp game/resource/Shader.hpp
        game/components/Transform.hpp
        game/Camera.cpp game/Camera.hpp
        game/components/Input.hpp
        game/Utility/CoordinateHelper.hpp game/Utility/CoordinateHelper.cpp
        game/components/Texture.hpp
        game/components/Animation.hpp)

# SFML
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
# OpenGL
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
# FreeType
target_link_libraries(${PROJECT_NAME} PRIVATE ${FREETYPE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
# ECS EnTT
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT)
# Cereal for serialization
target_include_directories(${PROJECT_NAME} PRIVATE external_includes/cereal/include)
