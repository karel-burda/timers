#pragma once

namespace burda
{
namespace timers
{
namespace policies
{
namespace start
{
/// Specifies how timer class will behave when exception is thrown from the user's callback
enum class exception : unsigned char
{
    /// Causes the timer to call "stop()" on itself and re-throws the catched exception
    stop = 0,
    /// Causes the timer to ignore catched exception and keep on
    ignore
};

constexpr exception get_default()
{
    return exception::stop;
}
}
}
}
}
