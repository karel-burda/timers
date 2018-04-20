#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/single_shot.h>

#include "static_assertions.h"
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

    timers::single_shot m_single_shot_timer;
    bool m_callback_called = false;
};

TEST(blocking_test_static, static_assertions)
{
    assert_properties<timers::single_shot>();

    SUCCEED();
}

TEST(single_shot_construction_destruction, basic_construction_destruction)
{
    ASSERT_NO_THROW(timers::single_shot single_shot_timer);

    timers::single_shot single_shot_timer;
    EXPECT_NO_THROW(single_shot_timer.timers::single_shot::~single_shot());
}

TEST_F(single_shot_test, callback_called)
{
    EXPECT_FALSE(m_callback_called);

    EXPECT_TRUE(m_single_shot_timer.start(3s, std::bind(&single_shot_test::callback, this)));

    EXPECT_TRUE(m_callback_called);
}

TEST_F(single_shot_test, callback_throwing)
{
    EXPECT_THROW(m_single_shot_timer.start(3s, nullptr), timers::exceptions::callback_not_callable);
}

TEST_F(single_shot_test, callback_multiple_times)
{
    const auto start = timers::testing::clock::now();

    EXPECT_TRUE(m_single_shot_timer.start(3s, std::bind(&single_shot_test::callback, this)));
    EXPECT_TRUE(m_single_shot_timer.start(3s, std::bind(&single_shot_test::callback, this)));

    const auto end = timers::testing::clock::now();
    const auto elapsed = timers::testing::round_to_seconds(end - start);

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 6.0, 100.0);
}

TEST_F(single_shot_test, stop)
{
    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_NO_THROW(m_single_shot_timer.stop());
        ASSERT_TRUE(m_single_shot_timer.m_terminated);
    }
}

TEST_F(single_shot_test, stop_before_callback)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        EXPECT_FALSE(m_single_shot_timer.start(5s, std::bind(&single_shot_test::callback, this)));
    });

    std::this_thread::sleep_for(1s);
    m_single_shot_timer.stop();

    EXPECT_FALSE(m_callback_called);

    caller.get();
}
}
