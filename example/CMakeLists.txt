cmake_minimum_required(VERSION 3.0)

project(example VERSION 0.9.5 LANGUAGES CXX)

include("${CMAKE_SOURCE_DIR}/cmake/settings.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

