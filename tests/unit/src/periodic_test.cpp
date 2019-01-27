#include <future>
#include <thread>

#include <gtest/gtest.h>

// deliberately in this place ahead of following includes
#include <test_utils/make_all_members_public.hpp>

#include <test_utils/lifetime.hpp>
#include <test_utils/macros.hpp>
#include <test_utils/mutex.hpp>
#include <test_utils/statics.hpp>
#include <timers/periodic.hpp>

namespace
{
using namespace std::chrono_literals;

namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

class periodic_test : public ::testing::Test
{
private:
    void callback()
    {
        ++m_counter;
    }

    unsigned char m_counter = 0;
    timers::periodic m_timer;
};

TEST(periodic_construction_destruction, construction_destruction)
{
    test_utils::lifetime::assert_construction_and_destruction<timers::periodic>();
}

TEST_F(periodic_test, static_assertions)
{
    test_utils::statics::assert_default_constructibility<decltype(m_timer), true>();
    test_utils::statics::assert_copy_constructibility<decltype(m_timer), false>();
    test_utils::statics::assert_move_constructibility<decltype(m_timer), false>();

    SUCCEED();
}

TEST_F(periodic_test, default_values)
{
    test_utils::mutex::check_if_owned(m_timer.m_cv_protection, false);
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
        m_timer.start(1s, [this](){ ++m_counter; throw std::exception{}; }, timers::policies::start::exception::stop);
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
        m_timer.start(1s, [this](){ ++m_counter; throw std::exception{}; }, timers::policies::start::exception::ignore);
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

TEST_F(periodic_test, start_in_parallel)
{
    bool taskFinished1 = false;
    bool taskFinished2 = false;

    auto starter1 = std::async(std::launch::async, [this, &taskFinished1]()
    {
        m_timer.start(1s, [&taskFinished1]() { taskFinished1 = true; });
    });
    auto starter2 = std::async(std::launch::async, [this, &taskFinished2]()
    {
        m_timer.start(1s, [&taskFinished2]() { taskFinished2 = true; });
    });

    std::this_thread::sleep_for(4s);

    m_timer.stop();
    starter1.wait();
    starter2.wait();

    // XOR relationship expected, because exactly one timer should have been started 
    BURDA_TEST_UTILS_EXPECT_XOR(taskFinished1, taskFinished2);
}

TEST_F(periodic_test, stop)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(3s);

    EXPECT_NO_THROW(m_timer.stop());
    EXPECT_TRUE(m_timer.m_terminate_forcefully);

    for (size_t i = 0; i < 100; ++i)
    {
        EXPECT_NO_THROW(m_timer.stop());
    }
}
}
