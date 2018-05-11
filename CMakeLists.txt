cmake_minimum_required(VERSION 3.0)

project(timers VERSION 0.9.5 LANGUAGES CXX)

include("cmake/settings.cmake")
include("cmake/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE CMAKE_RELATED cmake/*.cmake)
file(GLOB OTHER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "README.md")

option(EXAMPLE "Build example usage" ON)
option(UNIT-TESTS "Build unit tests" OFF)
option(PERFORMANCE-TESTS "Build performance tests" OFF)

if (EXAMPLE)
    message(STATUS "Building with example")

    add_subdirectory(example)
endif()

if (UNIT-TESTS)
    message(STATUS "Building with unit tests")

    enable_testing()
    add_subdirectory(tests/unit)
endif()

add_custom_target(${PROJECT_NAME} SOURCES ${HEADERS} ${CMAKE_RELATED} ${OTHER})

