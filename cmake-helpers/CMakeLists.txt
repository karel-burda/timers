cmake_minimum_required(VERSION 3.0)

project(cmake-helpers VERSION 0.9.5 LANGUAGES NONE)

file(GLOB_RECURSE CMAKE_RELATED *.cmake)

add_custom_target(${PROJECT_NAME} SOURCES ${CMAKE_RELATED})
