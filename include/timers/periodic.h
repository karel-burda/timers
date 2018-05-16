#pragma once

#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
/// Thread-safe blocking timer that does given action in a regular fashion during fixed time intervals
class periodic : public single_shot
{
public:
    /// Starts periodic timer using "single_shot::start" method 
    /// Blocks current thread
    /// @see "single_shot::start"
    void start(interval interval, callback callback, policies::start::exception policy = policies::start::get_default())
    {
        std::lock_guard<decltype(m_start_protection)> lock { m_start_protection };

        while(single_shot::start(interval, callback, policy));
    }

private:
    std::mutex m_start_protection;
};
}
}
