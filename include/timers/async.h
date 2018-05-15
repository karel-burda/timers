#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <type_traits>

#include "timers/type_definitions.h"
#include "timers/policies.h"

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
    void start(time_interval interval, timers_callback callback, policies::start::exception policy = policies::start::get_default())
    {
        std::lock_guard<decltype(m_async_protection)> lock { m_async_protection };

        m_async_task = std::async(std::launch::async, &underlying_timer::start, this, interval, std::move(callback), policy);
    }

    void stop()
    {
        std::lock_guard<decltype(m_async_protection)> lock { m_async_protection };

        underlying_timer::stop();

        m_async_task.wait();
    }

private:
    std::future<void> m_async_task;
    std::mutex m_async_protection;
};
}
}
