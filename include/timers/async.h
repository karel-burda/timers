#pragma once

#include "timers/blocking.h"

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
        std::lock_guard<decltype(m_protection)> lock{ m_protection };

        m_async_task = std::async(std::launch::async, &underlying_timer::start, this, interval, std::move(callback));

        return false;
    }

    void stop() override
    {
        std::lock_guard<decltype(m_protection)> lock{ m_protection };

        underlying_timer::stop();

        m_async_task.get();
    }

private:
    std::future<bool> m_async_task;
    std::mutex m_protection;
};
}
}
