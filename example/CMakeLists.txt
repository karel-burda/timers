cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example VERSION 1.1.1 LANGUAGES CXX)

_print_status_message("building with example")
_print_project_version()

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

_add_pedantic_warning_level(${PROJECT_NAME})
_link_threads_if_available(${PROJECT_NAME})
