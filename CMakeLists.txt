cmake_minimum_required(VERSION 3.0)

project(timers VERSION 0.9.1 LANGUAGES CXX)

include("cmake/settings.cmake")
include("cmake/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)
file(GLOB_RECURSE CMAKE_RELATED cmake/*.cmake)
file(GLOB OTHER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "README.md")

include_directories(include)

option(TESTS "Build all unit tests" OFF)
if (TESTS)
    find_package(gtest MODULE REQUIRED)
    enable_testing()
endif()

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${CMAKE_RELATED} ${OTHER})
