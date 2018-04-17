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

struct time_period_invalid : std::exception
{
};

struct time_period_zero : time_period_invalid
{
};

struct time_period_negative : time_period_invalid
{
};
}
}
}
