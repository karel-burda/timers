cmake_minimum_required(VERSION 3.0)

project(timers VERSION 1.2.0 LANGUAGES CXX)

include("cmake-helpers/cmake-helpers/messages.cmake")
include("cmake-helpers/cmake-helpers/standard-settings.cmake")
include("cmake-helpers/cmake-helpers/threads.cmake")
include("cmake-helpers/cmake-helpers/warnings.cmake")

_print_project_version()

_set_standards("11" "14")
_set_default_build_type("Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB OTHER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "README.md" ".travis.yml")

option(EXAMPLE "Build example usage" ON)
option(UNIT-TESTS "Build unit tests" OFF)
option(PERFORMANCE-TESTS "Build performance tests" OFF)

if (EXAMPLE)
    add_subdirectory(example)
endif()

if (UNIT-TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()

add_custom_target(${PROJECT_NAME} SOURCES ${HEADERS} ${OTHER})
