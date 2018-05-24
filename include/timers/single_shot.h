#pragma once

#include "timers/blocking.h"

#include "timers/policies.h"

namespace burda
{
namespace timers
{
/// Thread-safe blocking timer that performs defined action after given time 
class single_shot : public blocking
{
public:
    /// Starts timer and invokes callback after given time interval
    /// Blocks current thread
    /// @throws callback_is_not_callable, time_period_is_negative, time_period_is_zero
    /// @see "blocking::start"
    /// @see "policies::start::exception::start" for information about policies
    /// @returns true if terminated "naturally", false if terminated forcefully using the "stop()"
    bool start(interval interval, callback callback, policies::start::exception policy = policies::start::get_default())
    {
        const auto terminated_after_time_elapsed = blocking::block(interval);

        if (terminated_after_time_elapsed)
        {
            call(callback, policy);
        }

        return terminated_after_time_elapsed;
    }

private:
    void call(callback callback, policies::start::exception policy)
    {
        if (callback)
        {
            try
            {
                callback();
            }
            catch (...)
            {
                if (policy == policies::start::exception::stop)
                {
                    blocking::stop();

                    throw;
                }
            }
        }
        else
        {
            throw exceptions::callback_not_callable{};
        }
    }
};
}
}
