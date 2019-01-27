#include <future>
#include <thread>

#include <gtest/gtest.h>

#include <cpp_utils/time/measure_duration.hpp>
#include <test_utils/make_all_members_public.hpp>
#include <test_utils/lifetime.hpp>
#include <test_utils/mutex.hpp>
#include <test_utils/statics.hpp>
#include <test_utils/time.hpp>
#include <timers/blocking.hpp>
#include <timers/exceptions.hpp>

namespace
{
using namespace std::chrono_literals;

namespace cpp_utils = burda::cpp_utils;
namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

class blocking_test : public ::testing::Test
{
private:
    timers::blocking m_timer;
};

TEST(blocking_construction_destruction, construction_destruction)
{
    test_utils::lifetime::assert_construction_and_destruction<timers::blocking>();
}

TEST_F(blocking_test, static_assertions)
{
    test_utils::statics::assert_default_constructibility<decltype(m_timer), true>();
    test_utils::statics::assert_copy_constructibility<decltype(m_timer), false>();
    test_utils::statics::assert_move_constructibility<decltype(m_timer), false>();

    SUCCEED();
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_timer.m_terminate_forcefully);
    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
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
    const auto elapsed = cpp_utils::time::measure_duration([this]()
    {
        EXPECT_TRUE(m_timer.block(2s));
    });

    test_utils::time::assert_that_elapsed_time_in_tolerance(elapsed, 2s, 100s);
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
    const auto elapsed = cpp_utils::time::measure_duration([this]()
    {
        EXPECT_TRUE(m_timer.block(1s));
        EXPECT_TRUE(m_timer.block(4s));
    });

    test_utils::time::assert_that_elapsed_time_in_tolerance(elapsed, 5s, 100s);
    EXPECT_FALSE(m_timer.m_terminate_forcefully);
}

TEST_F(blocking_test, block_multiple_times_in_parallel)
{
    const auto elapsed = cpp_utils::time::measure_duration([this]()
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
    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::time::assert_that_elapsed_time_in_tolerance(elapsed, 10s, 100s);
}

TEST_F(blocking_test, block_multiple_times_in_parallel_and_stop_second)
{
    const auto elapsed = cpp_utils::time::measure_duration([this]()
    {
        std::atomic<bool> caller1_started = { false };
        std::atomic<bool> caller1_is_called_first = { true };

        auto caller1 = std::async(std::launch::async, [this, &caller1_started, &caller1_is_called_first]()
        {
            caller1_started = true;

            const auto terminated_naturally = m_timer.block(5s);

            if (caller1_is_called_first)
            {
                EXPECT_TRUE(terminated_naturally);
            }
            else
            {
                EXPECT_FALSE(terminated_naturally);
            }
        });

        auto caller2 = std::async(std::launch::async, [this, &caller1_started, &caller1_is_called_first]()
        {
            if (!caller1_started)
            {
                caller1_is_called_first = false;
            }

            const auto terminated_naturally = m_timer.block(5s);

            if (caller1_is_called_first)
            {
                EXPECT_FALSE(terminated_naturally);
            }
            else
            {
                EXPECT_TRUE(terminated_naturally);
            }
        });

        auto stopper_of_second_one = std::async(std::launch::async, [this]()
        {
            std::this_thread::sleep_for(7s);
            m_timer.stop();
        });

        caller1.wait();
        caller2.wait();
        stopper_of_second_one.wait();
    });

    EXPECT_TRUE(m_timer.m_terminate_forcefully);
    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::time::assert_that_elapsed_time_in_tolerance(elapsed, 7s, 100s);
}

TEST_F(blocking_test, stop)
{
    EXPECT_NO_THROW(m_timer.stop());

    m_timer.m_terminate_forcefully = false;
    m_timer.stop();
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
}
}
