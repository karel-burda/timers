#include <chrono>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/blocking.h>

namespace
{

class blocking_test : public ::testing::Test
{
protected:
    timers::blocking m_blocking_timer;
};

TEST(blocking_test_suite, construction)
{
    ASSERT_NO_THROW(timers::blocking blocking_timer);
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_blocking_timer.m_terminated_by_client);
}

TEST_F(blocking_test, termination)
{
    EXPECT_NO_THROW(m_blocking_timer.terminate());

    m_blocking_timer.m_terminated_by_client = true;
    m_blocking_timer.terminate();
    EXPECT_TRUE(m_blocking_timer.m_terminated_by_client);
}
}
