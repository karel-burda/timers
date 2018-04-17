#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/blocking.h>
#include <timers/exceptions.h>

#include "common.h"

namespace
{
using namespace std::chrono_literals;

class blocking_test : public ::testing::Test
{
protected:
    burda::timers::blocking m_blocking_timer;
};

TEST(blocking_test_construction_destruction, basic_construction_destruction)
{
    ASSERT_NO_THROW(burda::timers::blocking blocking_timer);

    burda::timers::blocking blocking_timer;
    EXPECT_NO_THROW(blocking_timer.~blocking_timer());
}

TEST(blocking_test_construction_destruction, destruction_while_blocking)
{
    burda::timers::blocking blocking_timer;

    auto blocker = std::async(std::launch::async, [&blocking_timer]()
    {
        blocking_timer.block(5s);
    });

    std::this_thread::sleep_for(1s);
    EXPECT_NO_THROW(blocking_timer.~blocking_timer());

    blocker.get();
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_not_throwing)
{
    EXPECT_NO_THROW(m_blocking_timer.block(1ms));
}

TEST_F(blocking_test, block_throwing)
{
    EXPECT_THROW(m_blocking_timer.block(0h), burda::timers::exceptions::time_period_zero);
    EXPECT_THROW(m_blocking_timer.block(-1ms), burda::timers::exceptions::time_period_negative);
}

TEST_F(blocking_test, block_time)
{
    // TODO: move this measuring into some common macro
    const auto start = burda::timers::testing::clock::now();
    m_blocking_timer.block(2s);
    const auto end = burda::timers::testing::clock::now();
    const auto elapsed = burda::timers::testing::round_to_seconds(end - start);

    burda::timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 2.0, 100.0);
}

TEST_F(blocking_test, block_and_terminate)
{
    auto terminator = std::async(std::launch::async, [this]()
    {
        std::this_thread::sleep_for(2s);
        m_blocking_timer.terminate();
    });

    m_blocking_timer.block(10s);
    terminator.get();

    EXPECT_TRUE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_multiple_times)
{
    const auto start = burda::timers::testing::clock::now();
    m_blocking_timer.block(1s);
    m_blocking_timer.block(4s);
    const auto end = burda::timers::testing::clock::now();
    const auto elapsed = burda::timers::testing::round_to_seconds(end - start);

    burda::timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 5.0, 100.0);
    EXPECT_TRUE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_multiple_times_in_parallel)
{
    const auto start = burda::timers::testing::clock::now();

    auto caller1 = std::async(std::launch::async, [this]()
    {
        m_blocking_timer.block(5s);
    });

    auto caller2 = std::async(std::launch::async, [this]()
    {
        m_blocking_timer.block(5s);
    });

    caller1.get();
    caller2.get();

    const auto end = burda::timers::testing::clock::now();
    const auto elapsed = burda::timers::testing::round_to_seconds(end - start);

    burda::timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 10.0, 100.0);
}

TEST_F(blocking_test, termination)
{
    EXPECT_NO_THROW(m_blocking_timer.terminate());

    m_blocking_timer.m_terminated = false;
    m_blocking_timer.terminate();
    EXPECT_TRUE(m_blocking_timer.m_terminated);
}
}
