#pragma once

#include "timers/single_shot.hpp"

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
        while(single_shot::start(interval, callback, policy));
    }
};
}
}
