set(_GTEST_TARGET_NAME "gtest-build")
set(_GTEST_BUILD_VARIANT "Release")
set(_GTEST_SOURCE_DIR_NAME "gtest-src")
set(_GTEST_SOURCE_DIR "${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR_NAME}")

add_custom_target(${_GTEST_TARGET_NAME} SOURCES "")

# We cannot use "ExternalProject_Add", since there's no options to disable submodules
# and this causes problems mainly on Windows git
# TODO: Optimization
if(NOT EXISTS "${_GTEST_SOURCE_DIR}")
    _print_status_message("Download gtest to the directory ${_GTEST_TARGET_NAME}")

    # TODO: perform a shallow clone on specific tag, but the gtest is currently not build-able on MSVC (1.7.0 and 1.8.0)
    execute_process(COMMAND git clone https://github.com/google/googletest.git ${_GTEST_SOURCE_DIR_NAME}
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
add_custom_command(TARGET ${_GTEST_TARGET_NAME} PRE_BUILD
                    COMMAND git checkout -f 0957cce368316577aae5ddfffcb67f24621d69e7
                    WORKING_DIRECTORY ${_GTEST_SOURCE_DIR})
add_custom_command(TARGET ${_GTEST_TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -DBUILD_GMOCK:BOOL=OFF -DBUILD_GTEST:BOOL=ON -DCMAKE_BUILD_TYPE=${_GTEST_BUILD_VARIANT} .
                   WORKING_DIRECTORY ${_GTEST_SOURCE_DIR})
add_custom_command(TARGET ${_GTEST_TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} --build . --config ${_GTEST_BUILD_VARIANT}
                   WORKING_DIRECTORY ${_GTEST_SOURCE_DIR})

include_directories(SYSTEM ${_GTEST_SOURCE_DIR}/googletest/include)
link_directories(${PROJECT_NAME} ${_GTEST_SOURCE_DIR}/googletest)
link_directories(${PROJECT_NAME} ${_GTEST_SOURCE_DIR}/googletest/${_GTEST_BUILD_VARIANT})

macro(_add_compile_options_for_gtest)
    if (WIN32)
        target_compile_options(${PROJECT_NAME} PUBLIC "/MT")
    endif()
endmacro()
