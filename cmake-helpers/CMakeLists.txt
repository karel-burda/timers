cmake_minimum_required(VERSION 3.0)

project(cmake-helpers VERSION 1.0.0 LANGUAGES NONE)

_print_project_version()

file(GLOB_RECURSE CMAKE_RELATED *.cmake)

add_custom_target(${PROJECT_NAME} SOURCES ${CMAKE_RELATED})
