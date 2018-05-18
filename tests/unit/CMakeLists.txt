cmake_minimum_required(VERSION 3.0)

project(tests VERSION 0.9.5 LANGUAGES CXX)

message(STATUS "timers: building with unit tests")

include("${CMAKE_SOURCE_DIR}/cmake-helpers/gtest.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_dependencies(${PROJECT_NAME} gtest-build)
target_link_libraries(${PROJECT_NAME} gtestd gtest_maind)

if (COVERAGE)
    message(STATUS "timers: building unit tests with code coverage")

    # TODO: Detect whether is on the GNU
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fcoverage-mapping -fprofile-instr-generate")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_test(NAME all-unit-tests COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
add_custom_target(run-all-tests-verbose
                  COMMAND
                  ${CMAKE_CTEST_COMMAND} -V -C ${CMAKE_BUILD_TYPE} --timeout 300 DEPENDS ${PROJECT_NAME})

supress_cxx_compiler_warning("keyword-macro")
link_pthreads_if_available()
