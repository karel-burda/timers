#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic_async.h>
#include <timers/exceptions.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class periodic_async_test : public ::testing::Test
{
protected:
    timers::periodic_async m_timer;
};

TEST_F(periodic_async_test, static_assertions)
{
    timers::testing::assert_default_constructibility<decltype(m_timer)>();
    timers::testing::assert_copy_constructibility<decltype(m_timer)>();
    timers::testing::assert_move_constructibility<decltype(m_timer)>();

    SUCCEED();
}

TEST(periodic_async_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::periodic_async>();
}

TEST_F(periodic_async_test, start_exception_policy_ignore)
{
    unsigned char counter = 0;
    EXPECT_NO_THROW(m_timer.start(1s, [&counter]() { ++counter; throw std::exception{}; }, timers::callback_exception_policy::ignore));

    std::this_thread::sleep_for(4s);

    EXPECT_GT(counter, 1);
    m_timer.stop();
}

TEST_F(periodic_async_test, start_exception_policy_stop)
{
    unsigned char counter = 0;
    EXPECT_NO_THROW(m_timer.start(1s, [&counter]() { ++counter; throw std::exception{}; }, timers::callback_exception_policy::stop));

    std::this_thread::sleep_for(3s);

    timers::testing::check_if_mutex_is_owned(m_timer.m_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminated);

    EXPECT_EQ(counter, 1);
    m_timer.stop();
}

TEST_F(periodic_async_test, start_long_callback)
{
    unsigned char callback_counter = 0;
    EXPECT_NO_THROW(m_timer.start(1s, [&callback_counter]() { ++callback_counter; std::this_thread::sleep_for(10s); }, timers::callback_exception_policy::stop));

    std::this_thread::sleep_for(5s);

    m_timer.stop();

    EXPECT_EQ(callback_counter, 1);
}

TEST_F(periodic_async_test, stop_in_parallel)
{
    EXPECT_FALSE(m_timer.start(2s, [](){}));

    auto stopper1 = std::async(std::launch::async, [this]()
    {
        m_timer.stop();
    });

    auto stopper2 = std::async(std::launch::async, [this]()
    {
        m_timer.stop();
    });

    stopper1.wait();
    stopper2.wait();

    timers::testing::check_if_mutex_is_owned(m_timer.m_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminated);
}
}
