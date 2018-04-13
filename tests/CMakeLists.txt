cmake_minimum_required(VERSION 3.0)

project(timers-tests VERSION 0.9.1 LANGUAGES CXX)

include("../cmake/gtest.cmake")
include("../cmake/settings.cmake")
include("../cmake/warnings.cmake")

include_directories(include)

add_executable(${PROJECT_NAME} src/blocking.test.cpp src/periodic.test.cpp include/make_all_members_public.h)

add_dependencies(${PROJECT_NAME} gtest-build)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
add_custom_target(run-all-tests-verbose COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS ${PROJECT_NAME})
