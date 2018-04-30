#pragma once

#include "timers/blocking.h"
#include "timers/policies.h"

namespace burda
{
namespace timers
{
class single_shot : public blocking
{
public:
    virtual bool start(time_interval interval, timers_callback callback, callback_exception_policy policy = get_default_callback_policy())
    {
        const auto terminated_after_time_elapsed = blocking::block(interval);

        if (terminated_after_time_elapsed)
        {
            call_or_throw_if_callback_is_not_callable(callback, policy);
        }

        return terminated_after_time_elapsed;
    }

private:
    void call_or_throw_if_callback_is_not_callable(timers_callback callback, callback_exception_policy policy)
    {
        if (callback)
        {
            try
            {
                callback();
            }
            catch (...)
            {
                if (policy == callback_exception_policy::stop)
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
