#pragma once

#include <type_traits>

#include "timers/private/disable_copy_and_move.h"

namespace burda
{
namespace timers
{
class blocking;

template <typename underlying_timer>
class scoped : public disable_copy_and_move
{
static_assert(!std::is_same<underlying_timer, blocking>::value, "Blocking timer is allowed be scoped");

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
        return *m_timer;
    }

    explicit operator bool() const
    {
        return m_timer;
    }

    // TODO: Define copy, move, and swap semantics

private:
    underlying_timer m_timer;
};
}
}
