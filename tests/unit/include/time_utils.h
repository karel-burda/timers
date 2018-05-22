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

void assert_that_elapsed_time_in_tolerance(std::chrono::seconds elapsed, std::chrono::seconds lower_bound, std::chrono::seconds upper_bound);

std::chrono::seconds measure_time(std::function<void()> action);
}
}
}
