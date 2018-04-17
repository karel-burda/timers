#pragma once

#include "timers/blocking.h"
#include "timers/common.h"

namespace burda
{
namespace timers
{
class single_shot : public no_copy_and_move_operations
{
public:
    virtual void start(time_interval interval, timers_callback callback)
    {
        if (m_blocking_timer.block(interval))
        {
            call_or_throw_if_callback_is_not_callable(std::move(callback));
        }
    }

    virtual void stop()
    {
        m_blocking_timer.terminate();
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

    blocking m_blocking_timer;
};
}
}
