#include <atomic>
#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/single_shot_async.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class single_shot_async_test : public ::testing::Test
{
protected:
    void callback()
    {
        m_callback_called = true;
    }

    timers::single_shot_async m_timer;
    bool m_callback_called = false;
};

TEST_F(single_shot_async_test, static_assertions)
{
    timers::testing::assert_default_constructibility<timers::single_shot_async>();
    timers::testing::assert_copy_constructibility<timers::single_shot_async>();
    timers::testing::assert_move_constructibility<timers::single_shot_async>();

    SUCCEED();
}

TEST(single_shot_async_construction_destruction, construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::single_shot_async>();
}

TEST_F(single_shot_async_test, default_values)
{
    EXPECT_FALSE(m_timer.m_async_task.valid());
    timers::testing::check_if_mutex_is_owned(m_timer.m_async_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
}

TEST_F(single_shot_async_test, callback_called)
{
    m_timer.start(2s, std::bind(&single_shot_async_test::callback, this));

    std::this_thread::sleep_for(5s);

    EXPECT_FALSE(m_timer.blocking::m_terminate_forcefully);
    EXPECT_TRUE(m_callback_called);
}

TEST_F(single_shot_async_test, start_exception_policy_stop)
{
    EXPECT_NO_THROW(m_timer.start(1s, [](){ throw std::exception{}; }, timers::policies::start::exception::stop));

    std::this_thread::sleep_for(4s);

    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_async_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
}

TEST_F(single_shot_async_test, callback_multiple_times)
{
    const auto start = timers::testing::clock::now();
    auto end = timers::testing::clock::now();

    m_timer.start(1s, [&end]()
    {
        end = timers::testing::clock::now();
    });
    m_timer.start(3s, [&end]()
    {
        end = timers::testing::clock::now();
    });
    m_timer.start(3s, [&end]()
    {
        end = timers::testing::clock::now();
    });

    std::this_thread::sleep_for(7s);

    timers::testing::assert_that_elapsed_time_in_tolerance(std::chrono::duration_cast<std::chrono::seconds>(end - start), 7s, 100s);
}

TEST_F(single_shot_async_test, start_in_parallel)
{
    std::atomic<unsigned char> counter = { 0 };
    bool taskFinished1 = false;
    bool taskFinished2 = false;

    m_timer.start(2s, [&counter, &taskFinished1]()
    {
        ++counter;
        taskFinished1 = true;
    });
    m_timer.start(2s, [&counter, &taskFinished2]()
    {
        ++counter;
        taskFinished2 = true;
    });

    while (!taskFinished1 || !taskFinished2)
    {
        std::this_thread::sleep_for(2s);
    }

    EXPECT_EQ(counter, 2);
}

TEST_F(single_shot_async_test, stop)
{
    m_timer.start(4s, std::bind(&single_shot_async_test::callback, this));

    std::this_thread::sleep_for(1s);

    m_timer.stop();

    EXPECT_TRUE(m_timer.blocking::m_terminate_forcefully);
    EXPECT_FALSE(m_callback_called);
}
}
