#include <chrono>

#include <gtest/gtest.h>

#include <timers/blocking.h>

namespace
{
TEST(blocking, basic)
{
    ASSERT_NO_THROW(timers::blocking blocking_timer);
}
}
