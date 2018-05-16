#pragma once

#include "timers/blocking.h"

namespace burda
{
namespace timers
{
class single_shot : public blocking
{
public:
    bool start(time_interval interval, timers_callback callback, policies::start::exception policy = policies::start::get_default())
    {
        const auto terminated_after_time_elapsed = blocking::block(interval);

        if (terminated_after_time_elapsed)
        {
            call(callback, policy);
        }

        return terminated_after_time_elapsed;
    }

private:
    void call(timers_callback callback, policies::start::exception policy)
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
