#pragma once

#include <exception>

namespace burda
{
namespace timers
{
namespace exceptions
{
struct callback_not_callable : public std::exception
{
};

struct time_period_is_invalid : public std::exception
{
};

struct time_period_is_zero : public time_period_is_invalid
{
};

struct time_period_is_negative : public time_period_is_invalid
{
};
}
}
}
