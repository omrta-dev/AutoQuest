# cmake_minimum_required(VERSION <specify CMake version here>)
project(AutoQuest)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS true)

find_package(SFML 2.5 COMPONENTS audio graphics network system window REQUIRED)
find_package(AWSSDK REQUIRED COMPONENTS core cognito-identity cognito-idp)
include_directories(external_includes/)

add_executable(${PROJECT_NAME} main.cpp aws/CognitoServer.cpp aws/CognitoServer.hpp aws/CognitoClient.cpp aws/CognitoClient.hpp)

# SFML
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
# AWS SDK
target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES})