#include "time_utils.h"

#include <stdexcept>

#include <gtest/gtest.h>

namespace burda
{
namespace timers
{
namespace testing
{
void assert_that_elapsed_time_in_tolerance(const std::chrono::duration<double> elapsed,
                                           const std::chrono::duration<double> lower_bound,
                                           const std::chrono::duration<double> upper_bound,
                                           const std::chrono::duration<double> epsilon)
{
    ASSERT_GE(elapsed + epsilon, lower_bound);
    ASSERT_LE(elapsed, upper_bound);
}

std::chrono::duration<double> measure_time(std::function<void()> action)
{
    const auto start = clock::now();

    if (action)
    {
        action();
    }
    else
    {
        throw std::runtime_error{"Given action is not callable"};
    }

    const auto end = clock::now();

    return end - start;
}
}
}
}
