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

TEST(single_shot_async_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::single_shot_async>();
}

TEST_F(single_shot_async_test, default_values)
{
    // TODO: find out why this crashes
    //EXPECT_TRUE(m_single_shot_async_timer.m_async_task.wait_for(0s) == std::future_status::ready);

    timers::testing::check_if_mutex_is_owned(m_timer.m_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
}

TEST_F(single_shot_async_test, callback_called)
{
    EXPECT_FALSE(m_timer.start(2s, std::bind(&single_shot_async_test::callback, this)));

    std::this_thread::sleep_for(5s);

    EXPECT_TRUE(m_timer.blocking::m_terminated);
    EXPECT_TRUE(m_callback_called);
}

TEST_F(single_shot_async_test, start_exception_policy_stop)
{
    EXPECT_NO_THROW(m_timer.start(1s, [](){ throw std::exception{}; }, timers::callback_exception_policy::stop));

    std::this_thread::sleep_for(4s);

    timers::testing::check_if_mutex_is_owned(m_timer.m_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminated);
}

TEST_F(single_shot_async_test, callback_multiple_times)
{
    const auto start = timers::testing::clock::now();
    auto end = timers::testing::clock::now();

    EXPECT_FALSE(m_timer.start(1s, [&end]()
    {
        end = timers::testing::clock::now();
    }));
    EXPECT_FALSE(m_timer.start(3s, [&end]()
    {
        end = timers::testing::clock::now();
    }));

    std::this_thread::sleep_for(5s);

    timers::testing::assert_that_elapsed_time_in_tolerance(timers::testing::round_to_seconds(end - start), 4.0, 100.0);
}

TEST_F(single_shot_async_test, start_in_parallel)
{
    std::atomic<unsigned char> counter = 0;
    bool taskFinished1 = false;
    bool taskFinished2 = false;

    EXPECT_FALSE(m_timer.start(2s, [&counter, &taskFinished1]()
    {
        ++counter;
        taskFinished1 = true;
    }));
    EXPECT_FALSE(m_timer.start(2s, [&counter, &taskFinished2]()
    {
        ++counter;
        taskFinished2 = true;
    }));

    while (!taskFinished1 && !taskFinished2);

    EXPECT_EQ(counter, 2);
}

TEST_F(single_shot_async_test, stop)
{
    m_timer.start(4s, std::bind(&single_shot_async_test::callback, this));

    std::this_thread::sleep_for(1s);

    m_timer.stop();

    EXPECT_TRUE(m_timer.blocking::m_terminated);
    EXPECT_FALSE(m_callback_called);
}
}
