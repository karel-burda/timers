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

    timers::single_shot_async m_single_shot_async_timer;
    bool m_callback_called = false;
};

TEST_F(single_shot_async_test, static_assertions)
{
    timers::testing::assert_properties<decltype(m_single_shot_async_timer)>();

    SUCCEED();
}

TEST(single_shot_async_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::single_shot_async>();
}

TEST_F(single_shot_async_test, callback_called)
{
    EXPECT_FALSE(m_single_shot_async_timer.start(2s, std::bind(&single_shot_async_test::callback, this)));

    std::this_thread::sleep_for(5s);

    EXPECT_TRUE(m_single_shot_async_timer.blocking::m_terminated);
    EXPECT_TRUE(m_callback_called);

    // TODO: test throwing callbacks
}

TEST_F(single_shot_async_test, callback_multiple_times)
{
    const auto start = timers::testing::clock::now();
    auto end = timers::testing::clock::now();

    EXPECT_FALSE(m_single_shot_async_timer.start(1s, [&end]()
    {
        end = timers::testing::clock::now();
    }));
    EXPECT_FALSE(m_single_shot_async_timer.start(3s, [&end]()
    {
        end = timers::testing::clock::now();
    }));

    timers::testing::assert_that_elapsed_time_in_tolerance(timers::testing::round_to_seconds(end - start), 4.0, 100.0);
}

TEST_F(single_shot_async_test, stop)
{
    m_single_shot_async_timer.start(4s, std::bind(&single_shot_async_test::callback, this));

    std::this_thread::sleep_for(1s);

    m_single_shot_async_timer.stop();

    EXPECT_TRUE(m_single_shot_async_timer.blocking::m_terminated);
    EXPECT_FALSE(m_callback_called);
}
}
