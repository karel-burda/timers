#pragma once

#include "timers/periodic.h"

#include <future>

namespace burda
{
namespace timers
{
class periodic_async : public periodic
{
public:
    void start(std::function<void()> callback) override
    {
        m_async_task = std::async(std::launch::async, [this, &callback]
        {
            periodic::start(std::move(callback));
        });
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
