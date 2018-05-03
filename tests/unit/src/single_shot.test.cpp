#include <chrono>
#include <exception>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/single_shot.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class single_shot_test : public ::testing::Test
{
protected:
    void callback()
    {
        m_callback_called = true;
    }

    timers::single_shot m_timer;
    bool m_callback_called = false;
};

TEST_F(single_shot_test, static_assertions)
{
    timers::testing::assert_default_constructibility<decltype(m_timer)>();
    timers::testing::assert_copy_constructibility<decltype(m_timer)>();
    timers::testing::assert_move_constructibility<decltype(m_timer)>();

    SUCCEED();
}

TEST(single_shot_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::single_shot>();
}

TEST_F(single_shot_test, callback_called)
{
    EXPECT_FALSE(m_callback_called);

    EXPECT_TRUE(m_timer.start(3s, std::bind(&single_shot_test::callback, this)));

    EXPECT_TRUE(m_callback_called);
}

TEST_F(single_shot_test, start_throwing)
{
    EXPECT_THROW(m_timer.start(0s, std::bind(&single_shot_test::callback, this)), timers::exceptions::time_period_is_zero);
    EXPECT_THROW(m_timer.start(-1h, std::bind(&single_shot_test::callback, this)), timers::exceptions::time_period_is_negative);
    EXPECT_THROW(m_timer.start(3s, nullptr), timers::exceptions::callback_not_callable);
}

TEST_F(single_shot_test, callback_throwing)
{
    EXPECT_ANY_THROW(m_timer.start(1s, [](){ throw std::exception{}; }));

    timers::testing::check_if_mutex_is_owned(m_timer.m_block_protection, false);
    timers::testing::check_if_mutex_is_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminated);
}

TEST_F(single_shot_test, callback_multiple_times)
{
    const auto elapsed = timers::testing::measure_time([this]()
    {
        EXPECT_TRUE(m_timer.start(3s, std::bind(&single_shot_test::callback, this)));
        EXPECT_TRUE(m_timer.start(3s, std::bind(&single_shot_test::callback, this)));
    });

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 6.0, 100.0);
}

TEST_F(single_shot_test, start_in_parallel)
{
    unsigned char counter = 0;
    auto starter1 = std::async(std::launch::async, [this, &counter]()
    {
        EXPECT_TRUE(m_timer.start(2s, [&counter]() { ++counter; }));
    });
    auto starter2 = std::async(std::launch::async, [this, &counter]()
    {
        EXPECT_TRUE(m_timer.start(2s, [&counter]() { ++counter; }));
    });

    starter1.wait();
    starter2.wait();
    m_timer.stop();

    EXPECT_EQ(counter, 2);
}

TEST_F(single_shot_test, stop)
{
    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_NO_THROW(m_timer.stop());
        ASSERT_TRUE(m_timer.m_terminated);
    }
}

TEST_F(single_shot_test, stop_before_callback)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        EXPECT_FALSE(m_timer.start(5s, std::bind(&single_shot_test::callback, this)));
    });

    std::this_thread::sleep_for(1s);
    m_timer.stop();

    EXPECT_FALSE(m_callback_called);

    caller.wait();
}
}
