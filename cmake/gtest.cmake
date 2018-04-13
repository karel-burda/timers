include(ExternalProject)

set(_GTEST_TARGET_NAME "gtest-build")
set(_GTEST_BUILD_VARIANT "Release")

ExternalProject_Add(${_GTEST_TARGET_NAME}
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           0957cce368316577aae5ddfffcb67f24621d69e7
  SOURCE_DIR        "${CMAKE_BINARY_DIR}/gtest-src"
  BINARY_DIR        "${CMAKE_BINARY_DIR}/gtest-src"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
ExternalProject_Get_Property(${_GTEST_TARGET_NAME} source_dir binary_dir install_dir)

message(STATUS "Running the cmake for the ${_GTEST_TARGET_NAME}")
add_custom_command(TARGET ${_GTEST_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=${_GTEST_BUILD_VARIANT} . WORKING_DIRECTORY ${source_dir})
message(STATUS "Running the make for the ${_GTEST_TARGET_NAME}")
add_custom_command(TARGET ${_GTEST_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} --build . --config ${_GTEST_BUILD_VARIANT} WORKING_DIRECTORY ${source_dir})

include_directories(${source_dir}/googletest/include)
link_directories(${PROJECT_NAME} ${binary_dir}/googlemock/gtest/${_GTEST_BUILD_VARIANT})

if (TESTS)
    if (WIN32)
        # gtest is always built in the Release, so just use "/MT" (not "/MTd")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    endif()
endif()
