#pragma once

#include <type_traits>

#include "timers/private/disable_copy.h"
#include "timers/private/disable_move.h"

namespace burda
{
namespace timers
{
class blocking;

/// RAII-like timer wrapper that automatically stops inner timer when going out of scope (during destructor)
/// @tparam underlying_timer might be whichever one except for the blocking
template <typename underlying_timer>
class scoped : private detail::disable_copy, private detail::disable_move
{
/// We allow only timers to be scoped except for the "blocking" that doesn't make sence
static_assert(!std::is_same<underlying_timer, blocking>::value, "Blocking timer is not allowed be scoped");
static_assert(std::is_base_of<blocking, underlying_timer>::value, "Only timers inherited from blocking are allowed be scoped");

public:
    ~scoped()
    {
        stop();
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
    void stop() noexcept
    {
        try
        {
            m_timer.stop();
        }
        catch (...)
        {
        }
    }

    underlying_timer m_timer;
};
}
}
