#pragma once

#include "timers/single_shot.h"

#include <future>

namespace burda
{
namespace timers
{
class single_shot_async : public single_shot
{
public:
    bool start(time_interval interval, timers_callback callback) override
    {
        m_async_task = std::async(std::launch::async, [this, interval, &callback]
        {
            single_shot::start(interval, std::move(callback));
        });

        return false;
    }

    void stop() override
    {
        single_shot::stop();

        m_async_task.get();
    }

private:
    std::future<void> m_async_task;
};
}
}
