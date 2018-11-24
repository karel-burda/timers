#pragma once

/// Origin: https://github.com/karel-burda/cpp-utils
/// Version: 1.0.0

#include <chrono>
#include <functional>
#include <stdexcept>

namespace burda
{
namespace cpp_utils
{
namespace time
{
inline std::chrono::duration<double> measure_duration(std::function<void()> action)
{
    const auto start = std::chrono::steady_clock::now();

    if (action)
    {
        action();
    }
    else
    {
        throw std::runtime_error{ "Given action is not callable" };
    }

    const auto end = std::chrono::steady_clock::now();

    return end - start;
}
}
}
}
