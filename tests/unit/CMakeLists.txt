cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(tests VERSION 1.1.1 LANGUAGES CXX)

_print_status_message("building with unit tests")
_print_project_version()

include("${CMAKE_SOURCE_DIR}/cmake-helpers/cmake-helpers/gtest.cmake")

file(GLOB_RECURSE HEADERS include/*.h*)
file(GLOB_RECURSE SOURCES src/*.c*)

_download_and_build_gtest("gtest-build" "Release" "gtest-src" "0957cce368316577aae5ddfffcb67f24621d69e7")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_dependencies(${PROJECT_NAME} gtest-build)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include include)
target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main)

if (COVERAGE)
    _print_status_message("building unit tests with code coverage")

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${PROJECT_NAME} PRIVATE "-fprofile-arcs" "-ftest-coverage" "-fcoverage-mapping" "-fprofile-instr-generate")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-fprofile-arcs -ftest-coverage")
    endif()
endif()

add_test(NAME all-unit-tests COMMAND $<TARGET_FILE:${PROJECT_NAME}> --gtest_color=yes --gtest_shuffle)
add_custom_target(run-all-tests-verbose
                  COMMAND ${CMAKE_CTEST_COMMAND} -V -C ${CMAKE_BUILD_TYPE} --timeout 300
                  DEPENDS ${PROJECT_NAME})

_add_compile_options_for_gtest()
_add_pedantic_warning_level()
_supress_cxx_compiler_warning("keyword-macro")
_link_threads_if_available()
