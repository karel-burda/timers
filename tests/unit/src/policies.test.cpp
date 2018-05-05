#include <gtest/gtest.h>

#include "timers/policies.h"

namespace
{
namespace timers = burda::timers;

TEST(policies_test, static_assertions)
{
    static_assert(timers::policies::start::get_default() == timers::policies::start::exception::stop,
                  "Incorrect default exception policy for the start(...) method");

    static_assert(timers::policies::stop::get_default() == timers::policies::stop::notification::all,
                  "Incorrect default notification policy for the stop(...) method");

    SUCCEED();
}
}
