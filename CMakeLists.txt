cmake_minimum_required(VERSION 3.17)

project(HtmlParser)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(./src/html_parser)
add_subdirectory(./src/interface)
add_subdirectory(./src/xpath_parser)
add_subdirectory(./src/css_selector)

add_executable(${PROJECT_NAME} ./src/main.cpp)

target_link_libraries(${PROJECT_NAME} html_parser)
target_link_libraries(${PROJECT_NAME} xpath_parser)
target_link_libraries(${PROJECT_NAME} interface)
target_link_libraries(${PROJECT_NAME} css_selector)
