#pragma once

#include <type_traits>

#include "timers/private/disable_copy.h"
#include "timers/private/disable_move.h"

namespace burda
{
namespace timers
{
class blocking;

template <typename underlying_timer>
class scoped : private detail::disable_copy, private detail::disable_move
{
/// We allow only timers to be scoped except for the "blocking" that doesn't make sence
static_assert(!std::is_same<underlying_timer, blocking>::value, "Blocking timer is not allowed be scoped");
static_assert(std::is_base_of<blocking, underlying_timer>::value, "Only timers inherited from blocking are allowed be scoped");

public:
    ~scoped()
    {
        m_timer.stop();
    }

    underlying_timer * operator->()
    {
        return &m_timer;
    }

    underlying_timer & operator*()
    {
        return m_timer;
    }

private:
    underlying_timer m_timer;
};
}
}
