cmake_minimum_required(VERSION 3.0)

project(timers-tests VERSION 0.9.1 LANGUAGES CXX)

include("../cmake/gtest.cmake")
include("../cmake/settings.cmake")
include("../cmake/warnings.cmake")

add_executable(${PROJECT_NAME} src/blocking.test.cpp)

add_dependencies(${PROJECT_NAME} gtest-build)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
