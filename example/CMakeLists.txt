cmake_minimum_required(VERSION 3.0)

project(example VERSION 0.9.5 LANGUAGES CXX)

message(STATUS "timers: building with the example")
print_project_version()

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_pedantic_warning_level()
link_pthreads_if_available()
