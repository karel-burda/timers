cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example LANGUAGES CXX)

message(STATUS "Building example")

include(${CMAKE_CURRENT_SOURCE_DIR}/../submodules/cmake-helpers/cmake-helpers/cpp_warnings.cmake)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
                   PRIVATE
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

find_package(timers CONFIG REQUIRED)
message(STATUS "Found version of timers is: ${timers_VERSION}")
target_link_libraries(${PROJECT_NAME} PRIVATE burda::timers)

burda_cmake_helpers_cpp_warnings_add_pedantic_level(${PROJECT_NAME} PRIVATE CXX)
