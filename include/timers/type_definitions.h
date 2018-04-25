#pragma once

#include <chrono>
#include <functional>

namespace burda
{
namespace timers
{
using time_interval = std::chrono::duration<double>;
using timers_callback = std::function<void()>;
}
}
