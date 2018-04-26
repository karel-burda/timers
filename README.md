# Introduction
**This is only alpha version that's just being developed**

`timers` features a header-only library that's implementing timer-related functionality and provides following features:
* General blocking timer: `block`
* Single-shot timer that does given action after period expires: `single_shot` and its asynchronous version `single_shot_async`
* Timer that does some action periodically: `periodic` and its asynchronous version `periodic_async`

# Requirements
* `timers` are implemented compatible with the C++11 standard and higher
* Example usage (`main.cpp`) is implemented on top of the C++14, because it uses neat `std::chrono_literals` which makes time specifications ease and read-able
* The above also applies for uni tests

# Technicals
Implemented using C++11 with the use of `std::conditional_variable`, `std::promise` and `std::async`.

Implementation might throw these exceptions:
* `callback_not_callable`
* `time_period_is_zero`
* `time_period_is_negative`

See `exceptions.h` for more info, if interested.

# Usage
TODO

For full use case, see `main.cpp` or implementation of unit tests at `tests/unit`

# Build Process
Library itself is just header-only, so no need for linking.

In order to build the usage example (`main.cpp`) run the cmake in the top-level directory: `cmake .`

I personally prefer to specify the separate build directory explicitly: `cmake -Bbuild -H.`

You can of course specify ordinary cmake options like build type (debug, release, ...), used generator, etc.

# Unit Tests
For building tests, run cmake with the option `UNIT-TESTS=ON`:
`cmake -Bbuild -H. -DUNIT-TESTS:BOOL=ON`

The project is using the `gtest` that is automatically downloaded, cmaked and built in its build step (the fixed stable revision of the `gtest` is used).

Then, you can run the default test target (e.g. `make test` or `RUN_TESTS` in the Visual Studio) or own custom target `run-all-tests-verbose` (which is recommended).

If you want to debug tests and beyond, run the target `timers-tests` manually.

# Branch Model
Project is using git workflow, this includes `master`, `develop`, feature (prefix `feature/`) and bugfix (`bugfix/`) branches, and `release/` for stable releases. 

# Coding Style
Following boost and STL coding guidelines with these exceptions:
* Using `m_` prefix for member variables
* Placing a space before and after the `*` or `&`
* Using `{}` instead of `()` to denote constructors, use `()` only for functions
* Include order is: interface file, standard includes, internal ones
