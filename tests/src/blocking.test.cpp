#include <chrono>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/blocking.h>

namespace
{
TEST(blocking, construction)
{
    ASSERT_NO_THROW(timers::blocking blocking_timer);

    timers::blocking blocking_timer;

    EXPECT_FALSE(blocking_timer.m_terminated_by_client);
}
}
