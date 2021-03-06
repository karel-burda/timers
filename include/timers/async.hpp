#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <type_traits>

#include "timers/policies.hpp"
#include "timers/type_definitions.hpp"

#include "timers/detail/type_traits.hpp"

namespace burda
{
namespace timers
{
class periodic;
class single_shot;

/// Thread-safe class enables "single_shot" and "periodic" timer behave in an asynchronous style
/// @tparam underlying_timer might be either "single_shot" or "blocking"
template <typename underlying_timer>
class async : public underlying_timer
{
static_assert(std::is_same<underlying_timer, periodic>::value || std::is_same<underlying_timer, single_shot>::value,
              "Only periodic or single_shot timers are allowed to behave in an asynchronous way");

public:
    /// Starts either single_shot or periodic action in an asynchronous way
    /// Callback is performed in separate thread
    /// @throws std::system_error if std::future can't be created
    /// @see "single_shot::start" and "periodic::start"
    void start(interval interval, callback callback, policies::start::exception policy = policies::start::get_default())
    {
        std::lock_guard<decltype(m_async_protection)> lock { m_async_protection };

        m_async_task = std::async(std::launch::async, &underlying_timer::start, this, interval, std::move(callback), policy);
    }

    /// Stops asynchronous timer and waits for the action to complete (if it's just running)
    /// @throws std::future_error
    void stop()
    {
        const std::lock_guard<decltype(m_async_protection)> lock { m_async_protection };

        underlying_timer::stop();

        if (m_async_task.valid())
        {
            m_async_task.wait();
        }
    }

private:
    std::future<detail::traits::return_type<decltype(&underlying_timer::start)>> m_async_task;
    std::mutex m_async_protection;
};
}
}
