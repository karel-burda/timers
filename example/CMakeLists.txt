cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(example VERSION 1.1.1 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})

_add_pedantic_warning_level()
_link_threads_if_available()
