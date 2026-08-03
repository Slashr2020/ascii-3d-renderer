cmake_minimum_required(VERSION 3.10)
project(Graphics C)

set(CMAKE_C_STANDART 11)
set(CMAKE_C_STANDART_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(graphics_app ${SOURCES})
target_link_libraries(graphics_app m)
