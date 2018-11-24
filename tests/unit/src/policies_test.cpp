#include <gtest/gtest.h>

#include <timers/policies.hpp>

namespace
{
namespace timers = burda::timers;

TEST(policies_test, static_assertions)
{
    static_assert(timers::policies::start::get_default() == timers::policies::start::exception::stop,
                  "Incorrect default exception policy for the start(...) method (should be 'stop')");

    SUCCEED();
}
}
