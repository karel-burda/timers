#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/blocking.h>
#include <timers/exceptions.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class blocking_test : public ::testing::Test
{
protected:
    timers::blocking m_blocking_timer;
};

TEST_F(blocking_test, static_assertions)
{
    timers::testing::assert_properties<decltype(m_blocking_timer)>();

    SUCCEED();
}

TEST(blocking_test_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::blocking>();
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_blocking_timer.m_terminated);
    timers::testing::check_whether_mutex_is_owned(m_blocking_timer.m_block_protection, false);
}

TEST_F(blocking_test, block_not_throwing)
{
    EXPECT_NO_THROW(m_blocking_timer.block(1ms));
}

TEST_F(blocking_test, block_throwing)
{
    EXPECT_THROW(m_blocking_timer.block(0h), timers::exceptions::time_period_is_zero);
    EXPECT_THROW(m_blocking_timer.block(-1ms), timers::exceptions::time_period_is_negative);
}

TEST_F(blocking_test, block_time)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        EXPECT_TRUE(m_blocking_timer.block(2s));
    });

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 2.0, 100.0);
}

TEST_F(blocking_test, block_and_stop)
{
    auto terminator = std::async(std::launch::async, [this]()
    {
        std::this_thread::sleep_for(2s);
        m_blocking_timer.stop();
    });

    EXPECT_FALSE(m_blocking_timer.block(10s));
    terminator.get();

    EXPECT_TRUE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_multiple_times)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        EXPECT_TRUE(m_blocking_timer.block(1s));
        EXPECT_TRUE(m_blocking_timer.block(4s));
    });

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 5.0, 100.0);
    EXPECT_TRUE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_multiple_times_in_parallel)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        auto caller1 = std::async(std::launch::async, [this]()
        {
            EXPECT_TRUE(m_blocking_timer.block(5s));
        });

        auto caller2 = std::async(std::launch::async, [this]()
        {
            EXPECT_TRUE(m_blocking_timer.block(5s));
        });

        timers::testing::check_whether_mutex_is_owned(m_blocking_timer.m_block_protection, true);

        caller1.get();
        caller2.get();
    });

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 10.0, 100.0);
}

TEST_F(blocking_test, stop)
{
    EXPECT_NO_THROW(m_blocking_timer.stop());

    m_blocking_timer.m_terminated = false;
    m_blocking_timer.stop();
    EXPECT_TRUE(m_blocking_timer.m_terminated);
    timers::testing::check_whether_mutex_is_owned(m_blocking_timer.m_block_protection, false);
}
}
