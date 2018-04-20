#pragma once

#include <exception>

namespace burda
{
namespace timers
{
namespace exceptions
{
struct callback_not_callable : std::exception
{
};

struct time_period_is_invalid : std::exception
{
};

struct time_period_is_zero : time_period_is_invalid
{
};

struct time_period_is_negative : time_period_is_invalid
{
};
}
}
}
