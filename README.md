[![Build Status](https://travis-ci.org/karel-burda/timers.svg?branch=feature%2Fci)](https://travis-ci.org/karel-burda/timers)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/27e08eaa6aa64eddbe4a79085e95ebcc)](https://www.codacy.com/app/karel-burda/timers?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=karel-burda/timers&amp;utm_campaign=Badge_Grade)
[![Coverity](https://scan.coverity.com/projects/15747/badge.svg)](https://scan.coverity.com/projects/karel-burda-timers)
[![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)](LICENSE)

# Introduction
**This is only alpha version that's just being developed**

`timers` features a header-only library that's implementing timer-related functionality and provides following features:
* General blocking timer: `block`
* Single-shot timer that does given action after period expires: `single_shot`
* Its asynchronous version `single_shot_async`
* Timer that does some action periodically: `periodic`
* Its asynchronous version `periodic_async`

# Requirements
* `timers` are implemented compatible with the C++11 standard and higher
* Example usage (`main.cpp`) is implemented on top of the C++14,
because it uses neat `std::chrono_literals` which makes time specifications easy and read-able
* The above also applies for unit tests

# Technical Info
Implemented using C++11 with the use of `std::conditional_variable`, `std::promise` and `std::async`.

Implementation might throw these exceptions:
* `callback_not_callable`
* `time_period_is_zero`
* `time_period_is_negative`

See `exceptions.h` for more info, if interested.

# Usage
In order to use the `timers`, it's only the `include` directory that matters. Just make sure that the header search
path is pointing to the `include` directory located in the root directory.

On systems that are using GNU compilers, you may need to link POSIX pthreads.
The project is using it in the build of example and unit tests using CMake: cmake-helpers/pthreads.cmake

TODO: Some most simple code in here

For full use case, see `main.cpp` or implementation of unit tests at `tests/unit`.

# Build Process
Library itself is just header-only, so no need for linking.

In order to build the usage example (`main.cpp`) run the cmake in the top-level directory:

`cmake .`

I personally prefer to specify the separate build directory explicitly:

`cmake -Bbuild -H.`

You can of course specify ordinary cmake options like build type (debug, release, ...), used generator, etc.

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

# Branch Model
Project is using git workflow, this includes `master`, `develop`, feature (prefix `feature/`)
and bug-fix (`bugfix/`) branches, and `release/` for stable releases. 

# Coding Style
Following boost and STL coding guidelines with these exceptions:
* Using `m_` prefix for member variables
* Placing a space before and after the `*` or `&`
* Using `{}` instead of `()` to denote constructors, using `()` only for function calls
* Include order is: interface file, standard includes, internal ones
