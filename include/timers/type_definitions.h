#pragma once

#include <chrono>
#include <functional>

namespace burda
{
namespace timers
{
using interval = std::chrono::duration<double>;
using callback = std::function<void()>;
}
}
