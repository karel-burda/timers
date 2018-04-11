cmake_minimum_required(VERSION 3.0)

project(timers-tests VERSION 0.9.1 LANGUAGES CXX)

#include("../cmake/settings.cmake")
#include("../cmake/warnings.cmake")

# Download and install GoogleTest
#project(googletest-download NONE)

include(ExternalProject)
ExternalProject_Add(gtest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           master
  SOURCE_DIR        "${CMAKE_BINARY_DIR}/gtest-src"
  BINARY_DIR        "${CMAKE_BINARY_DIR}/gtest-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir install_dir)

message(STATUS "${source_dir}, ${binary_dir}, ${install_dir}")

include_directories(${source_dir}/googletest/include)

enable_testing()

add_executable(basic_test src/blocking.test.cpp)

target_link_libraries(basic_test ${binary_dir}/libgtest)
target_link_libraries(basic_test ${binary_dir}/libgtest_main)
