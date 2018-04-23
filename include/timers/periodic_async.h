#pragma once

#include "timers/periodic.h"

#include <future>

namespace burda
{
namespace timers
{
// TODO: inherit from single_shot async? Or create generic abstract class "async"?
class periodic_async : public periodic
{
public:
    bool start(time_interval interval, timers_callback callback) override
    {
        m_async_task = std::async(std::launch::async, [this, interval, &callback]
        {
            periodic::start(interval, std::move(callback));
        });

        return false;
    }

    void stop() override
    {
        periodic::stop();

        m_async_task.get();
    }

private:
    std::future<void> m_async_task;
};
}
}
