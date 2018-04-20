#pragma once

#include <chrono>
#include <cmath>

#include <gtest/gtest.h>

namespace burda
{
namespace timers
{
namespace testing
{
using clock = std::chrono::steady_clock;
using time_interval = std::chrono::duration<double>;

inline void assert_that_elapsed_time_in_tolerance(time_interval elapsed, time_interval lower_bound, time_interval upper_bound)
{
    ASSERT_GE(elapsed, lower_bound);
    ASSERT_LE(elapsed, upper_bound);
}

inline void assert_that_elapsed_time_in_tolerance(double elapsed, double lower_bound, double upper_bound)
{
    ASSERT_GE(elapsed, lower_bound);
    ASSERT_LE(elapsed, upper_bound);
}

// TODO: dirty, write a template
inline double round_to_seconds(const std::chrono::nanoseconds & ns)
{
    return std::round(ns.count() / 1000.0 / 1000.0 / 1000.0);
}

inline double measure_time(std::function<void()> action)
{
    const auto start = timers::testing::clock::now();
    if (action)
    {
        action();
    }
    const auto end = timers::testing::clock::now();

    return timers::testing::round_to_seconds(end - start);
}
}
}
}
