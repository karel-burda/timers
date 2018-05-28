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
private:
    timers::blocking m_timer;
};

TEST(blocking_construction_destruction, construction_destruction)
{
    timers::testing::assert_construction_and_destruction<timers::blocking>();
}

TEST_F(blocking_test, static_assertions)
{
    timers::testing::assert_default_constructibility<decltype(m_timer)>();
    timers::testing::assert_copy_constructibility<decltype(m_timer)>();
    timers::testing::assert_move_constructibility<decltype(m_timer)>();

    SUCCEED();
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_timer.m_terminate_forcefully);
    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
}

TEST_F(blocking_test, block_not_throwing)
{
    EXPECT_NO_THROW(m_timer.block(1ms));
}

TEST_F(blocking_test, block_throwing)
{
    EXPECT_THROW(m_timer.block(0h), timers::exceptions::time_period_is_zero);
    EXPECT_THROW(m_timer.block(-1ms), timers::exceptions::time_period_is_negative);
}

TEST_F(blocking_test, block_time)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        EXPECT_TRUE(m_timer.block(2s));
    });

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 2s, 100s);
}

TEST_F(blocking_test, block_and_stop)
{
    auto terminator = std::async(std::launch::async, [this]()
    {
        std::this_thread::sleep_for(2s);
        m_timer.stop();
        EXPECT_TRUE(m_timer.m_terminate_forcefully);
    });

    EXPECT_FALSE(m_timer.block(10s));
    terminator.wait();

    EXPECT_TRUE(m_timer.m_terminate_forcefully);
}

TEST_F(blocking_test, block_multiple_times)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        EXPECT_TRUE(m_timer.block(1s));
        EXPECT_TRUE(m_timer.block(4s));
    });

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 5s, 100s);
    EXPECT_FALSE(m_timer.m_terminate_forcefully);
}

TEST_F(blocking_test, block_multiple_times_in_parallel)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        auto caller1 = std::async(std::launch::async, [this]()
        {
            EXPECT_TRUE(m_timer.block(5s));
        });

        auto caller2 = std::async(std::launch::async, [this]()
        {
            EXPECT_TRUE(m_timer.block(5s));
        });

        caller1.wait();
        caller2.wait();
    });

    EXPECT_FALSE(m_timer.m_terminate_forcefully);
    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 10s, 100s);
}

TEST_F(blocking_test, stop)
{
    EXPECT_NO_THROW(m_timer.stop());

    m_timer.m_terminate_forcefully = false;
    m_timer.stop();
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
}
}
