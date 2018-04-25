#pragma once

#include "timers/single_shot.h"

#include <future>
#include <memory>
#include <mutex>

namespace burda
{
namespace timers
{
template <typename underlying_timer>
class async : public underlying_timer
{
public:
    bool start(time_interval interval, timers_callback callback) override
    {
        std::lock_guard<decltype(m_protection)> lock { m_protection };

        m_async_task = std::async(std::launch::async, [this, interval, &callback]
        {
            underlying_timer::start(interval, std::move(callback));
        });

        return false;
    }

    void stop() override
    {
        std::lock_guard<decltype(m_protection)> lock { m_protection };

        underlying_timer::stop();

        m_async_task.get();
    }

private:
    std::future<void> m_async_task;
    std::mutex m_protection;
};
}
}
