#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "timers/disable_copy_and_move.h"
#include "timers/exceptions.h"
#include "timers/type_definitions.h"
#include "timers/policies.h"

namespace burda
{
namespace timers
{
class blocking : disable_copy_and_move
{
public:
    /// Waits and blocks current thread until the "time" elapses OR client code calls "terminate()"
    /// Returns false if terminated by the client
    bool block(time_interval time)
    {
        throw_if_time_invalid(time);

        std::lock_guard<decltype(m_block_protection)> lock{ m_block_protection };

        {
            std::unique_lock<decltype(m_cv_protection)> cv_lock{ m_cv_protection };

            const auto terminated_after_interval_elapsed = !m_cv.wait_for(cv_lock, time, [&]
            {
                return m_terminate_forcefully;
            });

            return terminated_after_interval_elapsed;
        }
    }

    /// Terminates timer, will set the "m_terminate_forcefully" to true, so that the cv stops to block
    void stop()
    {
        {
            std::lock_guard<decltype(m_cv_protection)> lock{ m_cv_protection };

            m_terminate_forcefully = true;
        }

        m_cv.notify_all();
    }

private:
    void throw_if_time_invalid(time_interval time) const
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
