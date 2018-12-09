cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(timers VERSION 1.3.2 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
                   INTERFACE
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/async.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/blocking.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/exceptions.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/periodic.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/periodic_async.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/policies.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/scoped.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/single_shot.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/single_shot_async.hpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/include/timers/type_definitions.hpp)

target_include_directories(${PROJECT_NAME}
                                INTERFACE
                                    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads QUIET)
if (Threads_FOUND)
    # Cannot use imported target Threads::Threads for interface (header only) library
    if (CMAKE_THREAD_LIBS_INIT)
        message(STATUS "Threading library found, will link to ${CMAKE_THREAD_LIBS_INIT}")
        set_target_properties(${PROJECT_NAME}
                                  PROPERTIES
                                      INTERFACE_LINK_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")
    endif()
endif()

target_compile_features(${PROJECT_NAME}
                            INTERFACE
                                cxx_std_11)

install(TARGETS ${PROJECT_NAME}
        EXPORT _targets)

export(EXPORT _targets
        NAMESPACE burda::
        FILE "${PROJECT_NAME}-config.cmake")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
                                 COMPATIBILITY ExactVersion)
