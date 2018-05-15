#pragma once

#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
class periodic : public single_shot
{
public:
    bool start(time_interval interval, timers_callback callback, policies::start::exception policy = policies::start::get_default())
    {
        std::lock_guard<decltype(m_start_protection)> lock { m_start_protection };

        while(single_shot::start(interval, callback, policy));

        return false;
    }

private:
    std::mutex m_start_protection;
};
}
}
