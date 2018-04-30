#pragma once

#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
class periodic : public single_shot
{
public:
    bool start(time_interval interval, timers_callback callback, callback_exception_policy policy = get_default_callback_policy()) override
    {
        while(single_shot::start(interval, callback, policy));

        // always has to be terminated from the outside
        return false;
    }
};
}
}
