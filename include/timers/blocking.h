#pragma once

#include <condition_variable>
#include <mutex>

#include "timers/exceptions.h"
#include "timers/type_definitions.h"

#include "timers/private/disable_copy.h"
#include "timers/private/disable_move.h"

namespace burda
{
namespace timers
{
/// Timer that blocks current thread for given amount of time
class blocking : private detail::disable_copy, private detail::disable_move
{
public:
    /// Waits and blocks current thread until the "time" elapses OR client code calls "stop()"
    /// @throws callback_is_not_callable, time_period_is_negative, time_period_is_zero
    /// @returns true if terminated "naturally", false if terminated forcefully using the "stop()"
    bool block(interval time)
    {
        throw_if_time_invalid(time);

        std::lock_guard<decltype(m_block_protection)> lock{ m_block_protection };

        std::unique_lock<decltype(m_cv_protection)> cv_lock{ m_cv_protection };
        const auto terminated_after_interval_elapsed = !m_cv.wait_for(cv_lock, time, [&]
        {
            return m_terminate_forcefully;
        });

        return terminated_after_interval_elapsed;
    }

    /// Stops the timer, will set the "m_terminate_forcefully" to true, so that the cv stops to block
    void stop()
    {
        {
            std::lock_guard<decltype(m_cv_protection)> lock{ m_cv_protection };

            m_terminate_forcefully = true;
        }

        m_cv.notify_all();
    }

private:
    void throw_if_time_invalid(interval time) const
    {
        if (time == time.zero())
        {
            throw exceptions::time_period_is_zero{};
        }
        else if (time < time.zero())
        {
            throw exceptions::time_period_is_negative{};
        }
    }

    std::condition_variable m_cv;
    std::mutex m_cv_protection;
    std::mutex m_block_protection;
    bool m_terminate_forcefully = false;
};
}
}
