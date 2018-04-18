#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "timers/common.h"
#include "timers/exceptions.h"

namespace burda
{
namespace timers
{
class blocking : public no_copy_and_move_operations
{
public:
    /// Waits and blocks current thread until the "time" elapses OR client code calls "terminate()"
    /// Returns false if terminated by the client
    bool block(time_interval time)
    {
        std::lock_guard<decltype(m_block_protection)> lock_block{ m_block_protection };

        throw_if_time_invalid(time);

        {
            std::unique_lock<decltype(m_protection)> lock{ m_protection };
            m_terminated = false;

            const auto terminated = !m_cv.wait_for(lock, time, [&]
            {
                return m_terminated;
            });

            m_terminated = true;

            return terminated;
        }
    }

    /// Terminates timer, will set the "m_terminated" to true, so that the cv stops to block
    void terminate()
    {
        {
            std::lock_guard<decltype(m_protection)> lock{ m_protection };

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
    std::mutex m_protection;
    std::mutex m_block_protection;
    bool m_terminated = false;
};
}
}
