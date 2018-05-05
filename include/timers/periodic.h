#pragma once

#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
class periodic : public single_shot
{
public:
    bool start(time_interval interval, timers_callback callback, policies::start::exception policy = policies::start::get_default()) override
    {
        while(single_shot::start(interval, callback, policy));

        // always has to be terminated from the outside
        return false;
    }
};
}
}
