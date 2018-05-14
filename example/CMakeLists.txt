cmake_minimum_required(VERSION 3.0)

project(example VERSION 0.9.5 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "This CMAKE is meant to be run from the top-level CMake in the root directory, run the top-level CMake instead")
endif()

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

link_pthreads_if_needed()

