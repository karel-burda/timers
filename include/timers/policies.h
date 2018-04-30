#pragma once

namespace burda
{
namespace timers
{
enum class callback_exception_policy : unsigned char
{
    stop,
    ignore
};

constexpr callback_exception_policy get_default_callback_policy()
{
    return callback_exception_policy::stop;
}
}
}
