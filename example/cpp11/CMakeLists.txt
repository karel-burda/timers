cmake_minimum_required(VERSION 3.0)

project(example-cpp11 VERSION 0.9.5 LANGUAGES CXX)

message(STATUS "timers: building with C++11 example")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

link_pthreads_if_available()

