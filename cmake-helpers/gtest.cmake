set(_GTEST_TARGET_NAME "gtest-build")
set(_GTEST_BUILD_VARIANT "Release")
set(_GTEST_SOURCE_DIR "gtest-src")

add_custom_target(${_GTEST_TARGET_NAME} SOURCES "")

# We cannot use "ExternalProject_Add", since there's no options to disable submodules
# that can cause some problems on some systems (e.g. this is buggy in Windows)
if(NOT EXISTS "${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR}")
    _print_status_message("Cleaning the download dir for the ${_GTEST_TARGET_NAME}")
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR})

    add_custom_command(TARGET ${_GTEST_TARGET_NAME} PRE_BUILD COMMAND git clone https://github.com/google/googletest.git ${_GTEST_SOURCE_DIR} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_command(TARGET ${_GTEST_TARGET_NAME} PRE_BUILD COMMAND git checkout -f 0957cce368316577aae5ddfffcb67f24621d69e7 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR})
endif()

add_custom_command(TARGET ${_GTEST_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -DBUILD_GMOCK:BOOL=OFF -DBUILD_GTEST:BOOL=ON -DCMAKE_BUILD_TYPE=${_GTEST_BUILD_VARIANT} . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR})
add_custom_command(TARGET ${_GTEST_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} --build . --config ${_GTEST_BUILD_VARIANT} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR})

include_directories(SYSTEM ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR}/googletest/include)
link_directories(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR}/googletest)
link_directories(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/${_GTEST_SOURCE_DIR}/googletest/${_GTEST_BUILD_VARIANT})

macro(_add_compile_options_for_gtest)
    if (WIN32)
        target_compile_options(${PROJECT_NAME} PUBLIC "/MT")
    endif()
endmacro()
