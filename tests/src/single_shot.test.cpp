#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/single_shot.h>

#include "common.h"

namespace
{
using namespace std::chrono_literals;

class single_shot_test : public ::testing::Test
{
protected:
    void callback()
    {
        m_callback_called = true;
    }

    burda::timers::single_shot m_single_shot_timer;
    bool m_callback_called = false;
};

TEST(single_shot_construction_destruction, basic_construction_destruction)
{
    ASSERT_NO_THROW(burda::timers::single_shot single_shot_timer);

    burda::timers::single_shot single_shot_timer;
    EXPECT_NO_THROW(single_shot_timer.burda::timers::single_shot::~single_shot());
}

TEST_F(single_shot_test, callback_called)
{
    EXPECT_FALSE(m_callback_called);

    m_single_shot_timer.start(3s, std::bind(&single_shot_test::callback, this));

    EXPECT_TRUE(m_callback_called);
}

TEST_F(single_shot_test, callback_throwing)
{
    EXPECT_THROW(m_single_shot_timer.start(3s, nullptr), burda::timers::exceptions::callback_not_callable);
}

TEST_F(single_shot_test, callback_multiple_times)
{
    const auto start = burda::timers::testing::clock::now();

    m_single_shot_timer.start(3s, std::bind(&single_shot_test::callback, this));
    m_single_shot_timer.start(3s, std::bind(&single_shot_test::callback, this));

    const auto end = burda::timers::testing::clock::now();
    const auto elapsed = burda::timers::testing::round_to_seconds(end - start);

    burda::timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 6.0, 100.0);
}

TEST_F(single_shot_test, stop)
{
    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_NO_THROW(m_single_shot_timer.stop());
        ASSERT_TRUE(m_single_shot_timer.m_blocking_timer.m_terminated);
    }
}

TEST_F(single_shot_test, stop_before_callback)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_single_shot_timer.start(5s, std::bind(&single_shot_test::callback, this));
    });

    m_single_shot_timer.stop();

    EXPECT_FALSE(m_callback_called);

    caller.get();
}
}
