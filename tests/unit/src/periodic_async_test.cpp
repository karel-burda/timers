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
#include <timers/exceptions.hpp>
#include <timers/periodic_async.hpp>

namespace
{
using namespace std::chrono_literals;

namespace cpp_utils = burda::cpp_utils;
namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

class periodic_async_test : public ::testing::Test
{
private:
    timers::periodic_async m_timer;
};

TEST(periodic_async_construction_destruction, construction_destruction)
{
    test_utils::lifetime::assert_construction_and_destruction<timers::periodic_async>();
}

TEST_F(periodic_async_test, static_assertions)
{
    test_utils::statics::assert_default_constructibility<decltype(m_timer), true>();
    test_utils::statics::assert_copy_constructibility<decltype(m_timer), false>();
    test_utils::statics::assert_move_constructibility<decltype(m_timer), false>();

    SUCCEED();
}

TEST_F(periodic_async_test, default_values)
{
    EXPECT_FALSE(m_timer.m_async_task.valid());
    test_utils::mutex::check_if_owned(m_timer.m_async_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
}

TEST_F(periodic_async_test, start_exception_policy_ignore)
{
    unsigned char counter = 0;
    EXPECT_NO_THROW(m_timer.start(1s, [&counter]() { ++counter; throw std::exception{}; }, timers::policies::start::exception::ignore));

    std::this_thread::sleep_for(4s);

    EXPECT_GT(counter, 1);
    m_timer.stop();
}

TEST_F(periodic_async_test, start_exception_policy_stop)
{
    unsigned char counter = 0;
    EXPECT_NO_THROW(m_timer.start(1s, [&counter]() { ++counter; throw std::exception{}; }, timers::policies::start::exception::stop));

    std::this_thread::sleep_for(3s);

    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_async_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminate_forcefully);

    EXPECT_EQ(counter, 1);
    m_timer.stop();
}

TEST_F(periodic_async_test, start_long_callback)
{
    const auto elapsed = cpp_utils::time::measure_duration([this]() {
        unsigned char callback_counter = 0;

        EXPECT_NO_THROW(m_timer.start(2s, [&callback_counter]()
        {
            ++callback_counter;
            std::this_thread::sleep_for(10s);
            ++callback_counter;
        }, timers::policies::start::exception::stop));

        std::this_thread::sleep_for(5s);

        m_timer.stop();

        EXPECT_EQ(callback_counter, 2);
    });

    test_utils::time::assert_that_elapsed_time_in_tolerance(elapsed, 12s, 100s);
}

TEST_F(periodic_async_test, stop_in_parallel)
{
    m_timer.start(2s, [](){});

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

    test_utils::mutex::check_if_owned(m_timer.m_block_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_async_protection, false);
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
    EXPECT_TRUE(m_timer.m_terminate_forcefully);
}
}
