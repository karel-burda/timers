#pragma once

#include <exception>
#include <functional>

#include "timers/blocking.h"
#include "timers/exceptions.h"
#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
class periodic : public single_shot
{
public:
    void start(time_interval interval, timers_callback callback) override
    {
        while (blocking::block(interval))
        {
            call_or_throw_if_callback_is_not_callable(std::move(callback));
        }
    }
};
}
}
