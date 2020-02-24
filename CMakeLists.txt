# cmake_minimum_required(VERSION <specify CMake version here>)
project(AutoQuest)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS true)

find_package(SFML 2.5 COMPONENTS audio graphics network system window REQUIRED)
find_package(AWSSDK REQUIRED COMPONENTS core cognito-identity cognito-idp)
include_directories(external_includes/glm/)
add_subdirectory(external_includes/glad/)

add_executable(${PROJECT_NAME} main.cpp aws/CognitoServer.cpp aws/CognitoServer.hpp aws/CognitoClient.cpp aws/CognitoClient.hpp game/Game.cpp game/Game.hpp game/InputManager.cpp game/InputManager.hpp game/ResourceManager.cpp game/ResourceManager.hpp game/Entity.cpp game/Entity.hpp game/glpp/VertexArrayObject.cpp game/glpp/VertexArrayObject.hpp game/glpp/VertexBufferObject.cpp game/glpp/VertexBufferObject.hpp game/glpp/Vertex.hpp)

# SFML
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
# OpenGL
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
# AWS SDK
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LINK_LIBRARIES})