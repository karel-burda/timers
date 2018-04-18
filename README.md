# Introduction
**This is only alpha version that's just being developed**

`timers` features a header-only library that's implementing timer-related functionality and provides following features:
* General blocking timer
* Timer that does some action periodically
* Periodic timer with asynchronous behaviour

# Technicals
Implemented using C++ 14 with the use of `std::conditional_variable`, `std::promise` and `std::async`.

# Usage
TODO

For full use case, see `main.cpp` or implementation of unit tests.

# Build Process
Library itself is just header-only, so no need for linking.

In order to build the usage example (`main.cpp`) run the cmake in the top-level directory: `cmake .`

I personally prefer to specify the separate build directory: `cmake -Bbuild -H.`

You can of course specify cmake options like build type (debug, release, ...), used generator, etc.

# Unit Tests
For building tests, run cmake with the option `tests=ON`:
`cmake -Bbuild -H. -DTESTS:BOOL=ON`

The project is using the `gtest` that is automatically downloaded, cmaked and built in its build step.

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
