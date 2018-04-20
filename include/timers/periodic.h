#pragma once

#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
class periodic : public single_shot
{
public:
    bool start(time_interval interval, timers_callback callback) override
    {
        while(single_shot::start(interval, std::move(callback)));

        // always has to be terminated from the outside
        return false;
    }
};
}
}
