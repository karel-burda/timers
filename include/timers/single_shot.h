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
    virtual void start(time_interval interval, timers_callback callback)
    {
        if (blocking::block(interval))
        {
            call_or_throw_if_callback_is_not_callable(std::move(callback));
        }
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
