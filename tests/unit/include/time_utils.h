#pragma once

#include <chrono>
#include <functional>

namespace burda
{
namespace timers
{
namespace testing
{
using clock = std::chrono::steady_clock;

void assert_that_elapsed_time_in_tolerance(std::chrono::duration<double> elapsed,
                                           std::chrono::duration<double> lower_bound,
                                           std::chrono::duration<double> upper_bound,
                                           std::chrono::duration<double> epsilon = std::chrono::milliseconds{ 10 });

std::chrono::duration<double> measure_time(std::function<void()> action);
}
}
}
