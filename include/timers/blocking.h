#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "timers/disable_copy_and_move.h"
#include "timers/exceptions.h"

namespace burda
{
namespace timers
{
using time_interval = std::chrono::duration<double>;
using timers_callback = std::function<void()>;

class blocking : disable_copy_and_move
{
public:
    /// Waits and blocks current thread until the "time" elapses OR client code calls "terminate()"
    /// Returns false if terminated by the client
    bool block(time_interval time)
    {
        std::lock_guard<decltype(m_block_protection)> lock_block{ m_block_protection };

        throw_if_time_invalid(time);

        {
            std::unique_lock<decltype(m_cv_protection)> lock{ m_cv_protection };
            m_terminated = false;

            const auto terminated_after_interval_elapsed = !m_cv.wait_for(lock, time, [&]
            {
                return m_terminated;
            });

            m_terminated = true;

            return terminated_after_interval_elapsed;
        }
    }

    /// Terminates timer, will set the "m_terminated" to true, so that the cv stops to block
    virtual void stop()
    {
        {
            std::lock_guard<decltype(m_cv_protection)> lock{ m_cv_protection };

            m_terminated = true;
        }

        m_cv.notify_all();
    }

private:
    void throw_if_time_invalid(time_interval time)
    {
        if (time == time.zero())
        {
            throw exceptions::time_period_zero{};
        }
        else if (time < time.zero())
        {
            throw exceptions::time_period_negative{};
        }
    }

    std::condition_variable m_cv;
    std::mutex m_cv_protection;
    std::mutex m_block_protection;
    bool m_terminated = false;
};
}
}
