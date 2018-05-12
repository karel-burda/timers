cmake_minimum_required(VERSION 3.0)

project(cmake-helpers VERSION 0.9.5 LANGUAGES NONE)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "This CMAKE is meant to be run from the top-level CMake in the root directory, run the top-level CMake instead")
endif()

file(GLOB_RECURSE CMAKE_RELATED *.cmake)

add_custom_target(${PROJECT_NAME} SOURCES ${CMAKE_RELATED})
