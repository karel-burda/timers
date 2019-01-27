#include <future>
#include <thread>

#include <gtest/gtest.h>

// deliberately in this place ahead of following includes
#include <test_utils/make_all_members_public.hpp>

#include <cpp_utils/time/measure_duration.hpp>
#include <test_utils/lifetime.hpp>
#include <test_utils/mutex.hpp>
#include <test_utils/statics.hpp>
#include <test_utils/time.hpp>
#include <timers/single_shot.hpp>

namespace
{
using namespace std::chrono_literals;

namespace cpp_utils = burda::cpp_utils;
namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

class single_shot_test : public ::testing::Test
{
private:
    void callback()
    {
        m_callback_called = true;
    }

    timers::single_shot m_timer;
    bool m_callback_called = false;
};

TEST(single_shot_construction_destruction, construction_destruction)
{
    test_utils::lifetime::assert_construction_and_destruction<timers::single_shot>();
}

TEST_F(single_shot_test, static_assertions)
{
    test_utils::statics::assert_default_constructibility<decltype(m_timer), true>();
    test_utils::statics::assert_copy_constructibility<decltype(m_timer), false>();
    test_utils::statics::assert_move_constructibility<decltype(m_timer), false>();

    SUCCEED();
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

    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
}

TEST_F(single_shot_test, callback_multiple_times)
{
    const auto elapsed = cpp_utils::time::measure_duration([this]()
    {
        EXPECT_TRUE(m_timer.start(3s, std::bind(&single_shot_test::callback, this)));
        EXPECT_TRUE(m_timer.start(3s, std::bind(&single_shot_test::callback, this)));
    });

    test_utils::time::assert_that_elapsed_time_in_tolerance(elapsed, 6s, 100s);
}

TEST_F(single_shot_test, start_long_callback)
{
    using clock = std::chrono::steady_clock;

    const auto start = clock::now();
    auto end = clock::now();

    EXPECT_NO_THROW(m_timer.start(2s, [&end]()
    {
        std::this_thread::sleep_for(10s);
        end = clock::now();
    }));

    const auto elapsed = end - start;

    test_utils::time::assert_that_elapsed_time_in_tolerance(std::chrono::duration_cast<std::chrono::seconds>(elapsed), 12s, 100s);
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
        ASSERT_TRUE(m_timer.m_terminate_forcefully);
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
