#pragma once

#include "timers/blocking.h"
#include "timers/common.h"

namespace burda
{
namespace timers
{
class single_shot : public blocking
{
public:
    virtual bool start(time_interval interval, timers_callback callback)
    {
        const auto terminated_after_time_elapsed = blocking::block(interval);

        if (terminated_after_time_elapsed)
        {
            call_or_throw_if_callback_is_not_callable(std::move(callback));
        }

        return terminated_after_time_elapsed;
    }

protected:
    void call_or_throw_if_callback_is_not_callable(timers_callback && callback)
    {
        if (callback)
        {
            callback();
        }
        else
        {
            throw exceptions::callback_not_callable{};
        }
    }
};
}
}
