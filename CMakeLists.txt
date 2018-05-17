cmake_minimum_required(VERSION 3.0)

project(timers VERSION 0.9.5 LANGUAGES CXX)

include("cmake-helpers/pthreads.cmake")
include("cmake-helpers/settings.cmake")
include("cmake-helpers/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB OTHER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "README.md" ".travis.yml")

option(EXAMPLE "Build example usage" ON)
option(UNIT-TESTS "Build unit tests" OFF)
option(PERFORMANCE-TESTS "Build performance tests" OFF)

add_subdirectory(cmake-helpers)

if (EXAMPLE)
    add_subdirectory(example)
endif()

if (UNIT-TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()

add_custom_target(${PROJECT_NAME} SOURCES ${HEADERS} ${OTHER})
