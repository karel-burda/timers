#include "time_utils.h"

#include <gtest/gtest.h>

namespace burda
{
namespace timers
{
namespace testing
{
void assert_that_elapsed_time_in_tolerance(const std::chrono::seconds elapsed, const std::chrono::seconds lower_bound, const std::chrono::seconds upper_bound)
{
    ASSERT_GE(elapsed, lower_bound);
    ASSERT_LE(elapsed, upper_bound);
}

std::chrono::seconds measure_time(std::function<void()> action)
{
    const auto start = timers::testing::clock::now();

    if (action)
    {
        action();
    }

    const auto end = timers::testing::clock::now();

    return std::chrono::duration_cast<std::chrono::seconds>(end - start);
}
}
}
}
