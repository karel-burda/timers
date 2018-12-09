![Version](https://img.shields.io/badge/version-1.3.2-green.svg)
[![License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/karel-burda/timers.svg?branch=develop)](https://travis-ci.org/karel-burda/timers)
[![Codecov Status](https://codecov.io/gh/karel-burda/timers/branch/develop/graph/badge.svg)](https://codecov.io/gh/karel-burda/timers/branch/develop)

# Important
This project contains git sub-modules that are needed for building example and tests.

If you just want to use the implementation, you can clone without sub-modules. In case you want to build the example or tests, be sure to clone the repository
with `--recurse-submodules` or `--recursive` on older versions of git. Alternatively, you can clone without sub-modules and initialize these later.

# Introduction
`timers` features a thread-safe and header-only library that's implementing timer-related functionality and provides following features:
* General blocking timer: `blocking`
* Single-shot timer that does given action after time period expires: `single_shot`
* Its asynchronous version: `single_shot_async`
* Timer that does some action periodically: `periodic`
* Its asynchronous version: `periodic_async`
* Scoped "RAII" timer that stops underlying timer automatically upon destruction: `scoped`

Implemented using C++11 with the use of `std::conditional_variable`, `std::promise` and `std::async`.

Client defines timer's action that should be of type `timers::callback`, thus `std::function<void()>`.
Time intervals are being defined as `timers::interval` = `std::chrono::duration<double>`.

Implementation might throw these exceptions upon the `start(...)`:
* `callback_not_callable`
* `time_period_is_zero`
* `time_period_is_negative`

See [exceptions.hpp](include/timers/exceptions.hpp) for more info.

Policies specifies how timer class will behave when exception is thrown from the user's callback:
* `stop` -- causes the timer to call `stop()` on itself and re-throws catched exception
* `ignore` -- causes the timer to ignore catched exception and keep on working

See [policies.hpp](include/timers/policies.hpp).

# Usage
Implementation resides in the `burda::timers` namespace, so it might be useful to do `namespace timers = burda::timers;` in your project.

## 1. CMake Way
Recommended option.

There are essentially these ways of how to use this package depending on your preferences our build architecture:

### A) Generate directly

Call `add_subdirectory(...)` directly in your CMakeLists.txt:

```cmake
add_executable(my-project main.cpp)

add_subdirectory(<path-to-timers>)
# example: add_subdirectory(timers ${CMAKE_BINARY_DIR}/timers)

# query of package version
message(STATUS "Current version of timers is: ${timers_VERSION}")

add_library(burda::timers ALIAS timers)

# this will import search paths, compile definitions and other dependencies of the timers as well
target_link_libraries(my-project timers)
# or with private visibility: target_link_libraries(my-project PRIVATE timers)
```

### B) Generate separately

Generation phase on the timers is run separately, that means that you run:
```cmake
cmake <path-to-timers>
# example: cmake -Bbuild/timers -Htimers in the root of your project 
```

This will create automatically generated package configuration file `timers-config.cmake` that contains exported target and all important information.

Then you can do this in your CMakeLists.txt:

```cmake
add_executable(my-project main.cpp)

find_package(timers CONFIG PATHS <path-to-binary-dir-of-timers>)
# alternatively assuming that the "timers_DIR" variable is set: find_package(timers CONFIG)

# you can also query (or force specific version during the previous "find_package()" call)
message(STATUS "Found version of timers is: ${timers_VERSION}")

# this will import search paths, compile definitions and other dependencies of the timers as well
target_link_libraries(my-project burda::timers)
# or with public visibility: target_link_libraries("my-project" PUBLIC burda::timers)
```

## 2. Manual Way
Not recommended.

Make sure that the `include` directory is in the search paths.

You also have to set C++ 11 standard and potentially other settings as well (e.g. linking `pthread` on POSIXes, etc.).

# Example
For full use cases, see [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

### Blocking
```cpp
timers::blocking timer;

// blocks current thread using condition_variable
// passing 5 seconds here, you can pass any chrono literal or std::chrono::duration manually
timer.block(5s);
// also possible to "stack" block() calls in a row (but from different thrads)

// from other thread you can terminate the timer (using timer.stop()),
// although this is not usually the case, since the main aim is the blocking behaviour itself
```

### Single-shot
```cpp
timers::single_shot timer;

// this call is blocking
timer.start(2s, [](){ std::cout << "Hi there" << std::endl; });

// ignore when exception is thrown
timer.start(2s, []()
{
    std::cout << "Gonna throw exception" << std::endl;
    throw std::runtime_error{};
}, timers::policies::start::ignore);

// these calls will throw exception:
timer.start(0s, [](){});
timer.start(-1h, [](){});
timer.start(2m, nullptr);
```

### Asynchronous single-shot
```cpp
timers::single_shot_async timer;

// this call is asynchronous
timer.start(2s, [](){ std::cout << "Hi there" << std::endl; });

// we can "stack" the callbacks
timer.start(1s, [](){ std::cout << "Action1" << std::endl; });
timer.start(1s, [](){ std::cout << "Action2" << std::endl; });
timer.start(1s, [](){ std::cout << "Action3" << std::endl; });

// will throw
timer.start(-5s, [](){});
```

### Periodic
```cpp
timers::periodic timer;

// blocking call
timer.start(3s, []() { std::cout << "This is being called regularly" << std::endl; });
// we should call "timer.stop()" from some other thread

// we can specify exception policy as well
 timer.start(3s, []() {}, timers::policies::start::ignore);

// this will throw anyway, the callback has to be callable
timer.start(5s, nullptr);
```

### Asynchronous periodic
```cpp
timers::periodic_async timer;

// non-blocking call
timer.start(3s, []() { std::cout << "This is being called regularly" << std::endl; });
// we can call "timer.stop()" right here theoretically
```

### Scoped
```cpp
class class_that_uses_timers
{
public:
    ~class_that_uses_timers()
    {
        // upon destruction, the scoped "m_timer" is destructed and calls "stop()"
        // on underlying timer (periodic async in this case)
    }

    void work()
    {
        // scoped timer has overloaded "->" and "*" operators
        m_timer->start(std::chrono::seconds{ 2 }, [](){ std::cout << "Hello!\n"; });
    }

private:
    timers::scoped<timers::periodic_async> m_timer;
};

class_that_uses_timers foo;
foo.work();

// "foo" goes out of scope, so the scoped timer (member of "foo") will go out of scope as well
// and its underlying asynchronous periodic timer will be stopped
```

For full use cases, see [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

# Unit Tests
Tests require sub-modules [cmake-helpers](https://github.com/karel-burda/cmake-helpers) and [test-utils](https://github.com/karel-burda/test-utils).

For building tests, run CMake in the source directory [tests/unit](tests/unit):

```cmake
cmake -Bbuild -H.

cmake -Bbuild/submodules/test-utils -Hsubmodules/test-utils
# you can also add coverage by appending "-DCOVERAGE:BOOL=ON"
cmake -Bbuild/tests/unit -Htests/unit
      -Dtimers_DIR:PATH=$(pwd)/build
      -Dtest-utils_DIR:PATH=$(pwd)/build/submodules/test-utils
cmake --build build/tests/unit

# this runs target "run-all-tests-verbose" that will also run the tests with timeout, etc.:
cmake --build build/tests/unit --target run-all-tests-verbose
```

This is the example of running tests in the debug mode.

For more info, see [.travis.yml](.travis.yml).

# Continuous Integration
Continuous Integration is now being run Linux, OS X and Windows on Travis: https://travis-ci.org/karel-burda/timers.

Compilers are set-up to treat warnings as errors and with pedantic warning level.
Targets are built in one stage with debug symbols with code coverage measure and in release mode with debug symbols in the second one.

Valgrind is being run on the example as well.

The project is using these jobs:
* `timers, example, tests -- linux, debug, cppcheck, valgrind, coverage, g++, 64-bit`
* `timers, example, tests -- osx, release with debug info, clang++, 64-bit`
* `timers, example, tests -- windows, release, msvc, 32-bit`

Project uses [codecov.io](https://codecov.io/gh/karel-burda/timers) for code coverage summary.
