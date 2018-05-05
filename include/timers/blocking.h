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

        {
            std::unique_lock<decltype(m_cv_protection)> lock{ m_cv_protection };

            const auto terminated_after_interval_elapsed = !m_cv.wait_for(lock, time, [&]
            {
                return m_terminate_forcefully;
            });

            m_terminate_forcefully = false;

            return terminated_after_interval_elapsed;
        }
    }

    /// Terminates timer, will set the "m_terminate_forcefully" to true, so that the cv stops to block
    virtual void stop(policies::stop::notification policy = policies::stop::get_default())
    {
        {
            std::lock_guard<decltype(m_cv_protection)> lock{ m_cv_protection };

            m_terminate_forcefully = true;
        }

        notify(policy);
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

    void notify(policies::stop::notification policy)
    {
        if (policy == policies::stop::notification::one)
        {
            m_cv.notify_one();
        }
        else if (policy == policies::stop::notification::all)
        {
            m_cv.notify_all();
        }
    }

    std::condition_variable m_cv;
    std::mutex m_cv_protection;
    bool m_terminate_forcefully = false;
};
}
}
