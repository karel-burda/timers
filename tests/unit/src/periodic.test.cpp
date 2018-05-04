#include <chrono>
#include <exception>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class periodic_test : public ::testing::Test
{
protected:
    void callback()
    {
        ++m_counter;
    }

    unsigned char m_counter = 0;
    timers::periodic m_timer;
};

TEST_F(periodic_test, static_assertions)
{
    timers::testing::assert_default_constructibility<decltype(m_timer)>();
    timers::testing::assert_copy_constructibility<decltype(m_timer)>();
    timers::testing::assert_move_constructibility<decltype(m_timer)>();

    SUCCEED();
}

TEST(periodic_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::periodic>();
}

TEST_F(periodic_test, callback_called)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(6s);

    m_timer.stop();
    caller.wait();

    EXPECT_GE(m_counter, 4);
}

TEST_F(periodic_test, start_exception_policy_stop)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_timer.start(1s, [this](){ ++m_counter; throw std::exception{}; }, timers::callback_exception_policy::stop);
    });

    std::this_thread::sleep_for(4s);
    m_timer.stop();
    caller.wait();

    EXPECT_EQ(m_counter, 1);
}

TEST_F(periodic_test, start_exception_policy_ignore)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_timer.start(1s, [this](){ ++m_counter; throw std::exception{}; }, timers::callback_exception_policy::ignore);
    });

    std::this_thread::sleep_for(4s);
    m_timer.stop();
    caller.wait();

    EXPECT_GT(m_counter, 1);
}

TEST_F(periodic_test, start_throwing)
{
    EXPECT_THROW(m_timer.start(0s, std::bind(&periodic_test::callback, this)), timers::exceptions::time_period_is_zero);
    EXPECT_THROW(m_timer.start(-1h, std::bind(&periodic_test::callback, this)), timers::exceptions::time_period_is_negative);
    EXPECT_THROW(m_timer.start(3s, nullptr), timers::exceptions::callback_not_callable);
}

// This is causing deadlock
TEST_F(periodic_test, start_in_parallel)
{
    bool taskFinished1 = false;
    bool taskFinished2 = false;

    auto starter1 = std::async(std::launch::async, [this, &taskFinished1]()
    {
        EXPECT_FALSE(m_timer.start(1s, [&taskFinished1]() { taskFinished1 = true; }));
    });
    auto starter2 = std::async(std::launch::async, [this, &taskFinished2]()
    {
        EXPECT_FALSE(m_timer.start(1s, [&taskFinished2]() { taskFinished2 = true; }));
    });

    std::this_thread::sleep_for(2s);

    m_timer.stop();
    starter1.wait();
    starter2.wait();

    // XOR relationship expected
    EXPECT_NE(!taskFinished1, !taskFinished2);
}

TEST_F(periodic_test, stop)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(3s);

    EXPECT_NO_THROW(m_timer.stop());
    EXPECT_TRUE(m_timer.m_terminated);

    for (size_t i = 0; i < 100; ++i)
    {
        EXPECT_NO_THROW(m_timer.stop());
    }
}
}
