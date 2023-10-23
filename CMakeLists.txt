cmake_minimum_required(VERSION 3.17)

project(html_parser)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(./src/parser)

add_executable(${PROJECT_NAME} ./src/main.cpp)

target_link_libraries(${PROJECT_NAME} parser)
