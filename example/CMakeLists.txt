cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(example VERSION 1.1.2 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_sources(${PROJECT_NAME} PUBLIC src/main.cpp)

_add_pedantic_warning_level()
_link_threads_if_available()
