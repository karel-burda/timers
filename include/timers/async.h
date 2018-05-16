#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <type_traits>

#include "timers/policies.h"
#include "timers/return_type_traits.h"
#include "timers/type_definitions.h"

namespace burda
{
namespace timers
{
class periodic;
class single_shot;

/// Class enables "single_shot" and "periodic" timer behave in an asynchronous style
/// @tparam underlying_timer might be either "single_shot" or "blocking"
template <typename underlying_timer>
class async : public underlying_timer
{
static_assert(std::is_same<underlying_timer, periodic>::value || std::is_same<underlying_timer, single_shot>::value,
              "Only periodic or single_shot timers are allowed to behave in an asynchronous way");

public:
    /// Starts either single_shot or periodic action in an asynchronous way
    /// Callback is performed in separate thread
    /// @throws callback_is_not_callable, time_period_is_negative, time_period_is_zero, std::system_error
    /// @see "single_shot::start" and "periodic::start"
    void start(interval interval, callback callback, policies::start::exception policy = policies::start::get_default())
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
    std::future<traits::return_type<decltype(&underlying_timer::start)>> m_async_task;
    std::mutex m_async_protection;
};
}
}
