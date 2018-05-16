[![Build Status](https://travis-ci.org/karel-burda/timers.svg?branch=feature%2Fci)](https://travis-ci.org/karel-burda/timers)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/27e08eaa6aa64eddbe4a79085e95ebcc)](https://www.codacy.com/app/karel-burda/timers?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=karel-burda/timers&amp;utm_campaign=Badge_Grade)
[![Coverity](https://scan.coverity.com/projects/15747/badge.svg)](https://scan.coverity.com/projects/karel-burda-timers)
[![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)](LICENSE)

# Introduction
`timers` features a thread-safe and header-only library that's implementing timer-related functionality and provides following features:
* General blocking timer: `block`
* Single-shot timer that does given action after timer period expires: `single_shot`
* Its asynchronous version `single_shot_async`
* Timer that does some action periodically: `periodic`
* Its asynchronous version `periodic_async`

Implemented using C++11 with the use of `std::conditional_variable`, `std::promise` and `std::async`.

Client defines timer's action that should be of type `timers::callback`, thus `std::function<void()>`.
Time intervals are being defined as `timers::interval` = `std::chrono::duration<double>`.

Implementation might throw these exceptions upon the `start(...)`:
* `callback_not_callable`
* `time_period_is_zero`
* `time_period_is_negative`

See [exceptions.h](include/timers/exceptions.h) for more info, if interested.

Policies specifies how timer class will behave when exception is thrown from the user's callback:
* `stop` -- causes the timer to call `stop()` on itself and re-throws the catched exception
* `ignore` -- causes the timer to ignore catched exception and keep on

See [policies.h](include/timers/policies.h).

# Usage
In order to use the `timers`, it's only the `include` directory that matters. Just make sure that the header search path is pointing to the [include](include) directory located in the root directory.

Implementation resides in the `burda::timers` namespace, so it might be useful to do `namespace timers = burda::timers;` in your project.

POSIX threads in the example usage and tests are being linked via CMake: [pthreads.cmake](cmake-helpers/pthreads.cmake)

See also section [Requirements](#Requirements).

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
timers::single_shot_async timer_async;

// this call is asynchronous
timer.start(2s, [](){ std::cout << "Hi there" << std::endl; });

// we can stack the commands
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

// will throw
timer.start(5s, nullptr);
}
```

### Asynchronous periodic
```cpp
timers::periodic_async timer;

// non-blocking call
timer.start(3s, []() { std::cout << "This is being called regularly" << std::endl; });
// we can call "timer.stop()" right here theoretically
```

For full use cases, [main.cpp](example/src/main.cpp) or implementation of unit tests at [tests/unit](tests/unit).

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
* `example` -- perform cppcheck on example usage (including `timers` themselves), build on gcc 5.x, run example under the valgrind
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
* Documentation of public interface is Doxygen-like
