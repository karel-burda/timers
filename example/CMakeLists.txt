cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(example LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../submodules/cmake-helpers/cmake-helpers/cpp_warnings.cmake)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
                   PRIVATE
                       ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)

find_package(timers CONFIG REQUIRED)
message(STATUS "Found version of timers is: ${timers_VERSION}")
target_link_libraries(${PROJECT_NAME} PRIVATE burda::timers)

burda_cmake_helpers_cpp_warnings_add_pedantic_level_cxx(${PROJECT_NAME} PRIVATE)
