#pragma once

namespace burda
{
namespace timers
{
namespace policies
{
namespace start
{
enum class exception : unsigned char
{
    stop,
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
