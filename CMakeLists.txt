cmake_minimum_required(VERSION 3.0)

project(timers VERSION 0.9.3 LANGUAGES CXX)

include("cmake/settings.cmake")
include("cmake/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)
file(GLOB_RECURSE CMAKE_RELATED cmake/*.cmake)
file(GLOB OTHER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "README.md")

include_directories(include)

option(UNIT-TESTS "Build unit tests" OFF)
option(PERFORMANCE-TESTS "Build unit tests" OFF)

if (UNIT-TESTS)
    message(STATUS "Building with unit tests")

    # "enable_testing()" has to be called in the top-level cmake
    enable_testing()
    add_subdirectory(tests/unit)
endif()

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${CMAKE_RELATED} ${OTHER})

