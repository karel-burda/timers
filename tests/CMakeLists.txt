cmake_minimum_required(VERSION 3.0)

project(timers-tests VERSION 0.9.1 LANGUAGES CXX)

include("../cmake/gtest.cmake")
include("../cmake/settings.cmake")
include("../cmake/warnings.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_dependencies(${PROJECT_NAME} gtest-build)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
add_custom_target(run-all-tests-verbose COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS ${PROJECT_NAME})
