[![Build Status](https://travis-ci.org/karel-burda/timers.svg?branch=feature%2Fci)](https://travis-ci.org/karel-burda/timers)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/27e08eaa6aa64eddbe4a79085e95ebcc)](https://www.codacy.com/app/karel-burda/timers?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=karel-burda/timers&amp;utm_campaign=Badge_Grade)
[![Coverity](https://scan.coverity.com/projects/15747/badge.svg)](https://scan.coverity.com/projects/karel-burda-timers)
[![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)](LICENSE)

# Introduction
`timers` features a thread-safe and header-only library that's implementing timer-related functionality and provides following features:
* General blocking timer: `block`
* Single-shot timer that does given action after period expires: `single_shot`
* Its asynchronous version `single_shot_async`
* Timer that does some action periodically: `periodic`
* Its asynchronous version `periodic_async`

# Requirements
* `timers` are implemented compatible with the C++11 standard and higher
* Example usage and unit tests is implemented on top of the C++14,
because it uses neat `std::chrono_literals` which makes time specifications easy and read-able
* Example usage and unit tests are using CMake (3.0 and higher required)
* On some systems, you may need to link POSIX pthreads.

# Implementation Info
Implemented using C++11 with the use of `std::conditional_variable`, `std::promise` and `std::async`.

Implementation might throw these exceptions:
* `callback_not_callable`
* `time_period_is_zero`
* `time_period_is_negative`

See [exceptions.h](include/timers/exceptions.h) for more info, if interested.
 
 TODO: policies

# Usage
In order to use the `timers`, it's only the `include` directory that matters. Just make sure that the header search
path is pointing to the [include](include) directory located in the root directory.

POSIX threads are being linked via CMake: [pthreads.cmake](cmake-helpers/pthreads.cmake)

```cpp
// blocking timer
{
    timers::blocking timer;

    // passing 5 seconds here, you can pass any chrono literal or std::chrono::duration manually
    timer.block(5s);

    // from other thread you can terminate the timer (using timer.stop()),
    // although this is not usually the case, since the main aim is the blocking behaviour itself
}


// single-shot timer
{
    timers::single_shot timer;
    timers::single_shot_async timer_async;

    // this call is blocking
    timer.start(2s, [](){ std::cout << "Hi there" << std::endl; });

    // this call is asynchronous
    timer.start(2s, [](){ std::cout << "Hi there" << std::endl; });

    // we could even "stack" the start() commands in a row, e.g.:
    //timer.start(1s, []() { std::cout << "1\n"; });
    //timer.start(2s, []() { std::cout << "1\n"; });
    //timer.start(3s, []() { std::cout << "1\n"; });
}

// periodic
{
    timers::periodic timer;
    timers::periodic_async timer_async;

    // blocking call
    timer.start(3s, []() { std::cout << "Hi there" << std::endl; });
    // we should call "timer.stop()" from some other thread

    // asynchronous call
    timer_async.start(3s, []() { std::cout << "Hi there" << std::endl; });
    timer_async.stop();
}

// TODO: policies
```

For full use case, [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

# Build Process
Library itself is just header-only, so no need for linking.

In order to build the usage example ([main.cpp](example/src/main.cpp)) run the cmake in the top-level directory like this:

`cmake .`

I personally prefer to specify a separate build directory explicitly:

`cmake -Bbuild -H.`

You can of course specify ordinary cmake options like build type (debug, release with debug info, ...), used generator, etc.

# Unit Tests
For building tests, run cmake with the option `UNIT-TESTS=ON`:

`cmake -Bbuild -H. -DUNIT-TESTS:BOOL=ON`

The project is using the `gtest` that is automatically downloaded, cmaked and built in its build step
(the fixed stable revision of the `gtest` is used).

Then, you can run the default test target (e.g. `make test` or `RUN_TESTS` in the Visual Studio)
or own custom target `run-all-tests-verbose` (which is recommended).

If you want to debug tests and beyond, run the target `timers-tests` manually.

It is also possible to turn off build of the example, and build just the tests:

`cmake -Bbuild -H. -DEXAMPLE:BOOL=OFF -DUNIT-TESTS:BOOL=ON`

# Continuous Integration
Continuous Integration is now being run OS X (clang 8.x) and Linux (gcc 5.x) on Travis: https://travis-ci.org/karel-burda/timers

Compilers are set-up to treat warnings as errors and compiler with the pedantic warning level and build targets as a release type of build (`example` is built as a release with debug symbols because of the valgrind).

The project is using free Travis services, so the CI process is (because of overhead and expense) broken up into just 2 steps (both with different OS & compiler):
* `example` -- perform cppcheck on example usage (including `timers themselves`), build on gcc 5.x, run example under the valgrind
* `tests` -- perform cppcheck on unit tests, build tests on clang 8.x, run tests

Project also uses https://app.codacy.com/app/karel-burda/timers/dashboard.

# Branch Model
Project is using git workflow, this includes `master`, `develop`, feature (prefix `feature/`)
and bug-fix (`bugfix/`) branches, and `release/` for stable releases. 

# Coding Style
Following boost and STL coding guidelines with these exceptions:
* Using `m_` prefix for member variables
* Placing a space before and after the `*` or `&`
* Using `{}` instead of `()` to denote constructors, using `()` only for function calls
* Include order is: interface file, standard includes, internal ones
