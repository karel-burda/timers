#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <type_traits>

#include "timers/type_definitions.h"

namespace burda
{
namespace timers
{
class periodic;
class single_shot;

template <typename underlying_timer>
class async : public underlying_timer
{
static_assert(std::is_same<underlying_timer, periodic>::value || std::is_same<underlying_timer, single_shot>::value,
              "Only periodic or single_shot timers are allowed to behave in an asynchronous way");

public:
    bool start(time_interval interval, timers_callback callback) override
    {
        std::lock_guard<decltype(m_protection)> lock { m_protection };

        m_async_task = std::async(std::launch::async, [this, interval, callback]
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
