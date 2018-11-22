#include <future>
#include <thread>

#include <gtest/gtest.h>

#include <test_utils/make_all_members_public.hpp>
#include <test_utils/lifetime_assertions.hpp>
#include <test_utils/mutex.hpp>
#include <test_utils/static_class_assertions.hpp>
#include <test_utils/time.hpp>
#include <timers/blocking.hpp>
#include <timers/exceptions.hpp>

#include "cpp_utils/time/measure_time.hpp"

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
    test_utils::assert_construction_and_destruction<timers::blocking>();
}

TEST_F(blocking_test, static_assertions)
{
    test_utils::assert_default_constructibility<decltype(m_timer), true>;
    test_utils::assert_copy_constructibility<decltype(m_timer), false>;
    test_utils::assert_move_constructibility<decltype(m_timer), false>;

    SUCCEED();
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_timer.m_terminate_forcefully);
    test_utils::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    test_utils::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
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

    test_utils::assert_that_elapsed_time_in_tolerance(elapsed, 2s, 100s);
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

    test_utils::assert_that_elapsed_time_in_tolerance(elapsed, 5s, 100s);
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
    test_utils::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    test_utils::assert_that_elapsed_time_in_tolerance(elapsed, 10s, 100s);
}

TEST_F(blocking_test, block_multiple_times_in_parallel_and_stop_second)
{
    const auto elapsed = cpp_utils::time::measure_duration([this]()
    {
        auto caller1 = std::async(std::launch::async, [this]()
        {
            EXPECT_TRUE(m_timer.block(5s));
        });

        auto caller2 = std::async(std::launch::async, [this]()
        {
            EXPECT_FALSE(m_timer.block(5s));
        });

        auto stopper_of_second = std::async(std::launch::async, [this]()
        {
            std::this_thread::sleep_for(7s);
            m_timer.stop();
        });

        caller1.wait();
        caller2.wait();
        stopper_of_second.wait();
    });

    EXPECT_TRUE(m_timer.m_terminate_forcefully);
    test_utils::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    test_utils::assert_that_elapsed_time_in_tolerance(elapsed, 7s, 100s);
}

TEST_F(blocking_test, stop)
{
    EXPECT_NO_THROW(m_timer.stop());

    m_timer.m_terminate_forcefully = false;
    m_timer.stop();
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
    test_utils::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    test_utils::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
}
}
