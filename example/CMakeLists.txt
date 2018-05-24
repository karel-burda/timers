cmake_minimum_required(VERSION 3.0)

project(example VERSION 0.9.9 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

_add_pedantic_warning_level()
_link_pthreads_if_available()
