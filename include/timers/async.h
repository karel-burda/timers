#pragma once

#include "timers/blocking.h"

#include <future>

namespace burda
{
namespace timers
{
template <typename underlying_timer>
class async : disable_copy_and_move
{
public:
    bool start(time_interval interval, timers_callback callback)
    {
        m_async_task = std::async(std::launch::async, [this, interval, &callback]
        {
            underlying_timer::start(interval, std::move(callback));
        });

        return false;
    }

    void stop()
    {
        underlying_timer::stop();

        m_async_task.get();
    }

private:
    std::future<void> m_async_task;
};
}
}
