#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class periodic_test : public ::testing::Test
{
protected:
    void callback()
    {
        ++m_counter;
    }

    unsigned char m_counter = 0;
    timers::periodic m_periodic_timer;
};

TEST_F(periodic_test, static_assertions)
{
    timers::testing::assert_properties<decltype(m_periodic_timer)>();

    SUCCEED();
}

TEST(periodic_construction_destruction, basic_construction_destruction)
{
    timers::testing::assert_construction_and_destruction<burda::timers::periodic>();
}

TEST_F(periodic_test, callback_called)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_periodic_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(6s);
    caller.get();

    EXPECT_GE(m_counter, 5);

    m_periodic_timer.stop();
}

TEST_F(periodic_test, callback_throwing)
{
    EXPECT_THROW(m_periodic_timer.start(0s, std::bind(&periodic_test::callback, this)), timers::exceptions::time_period_is_zero);
    EXPECT_THROW(m_periodic_timer.start(-1h, std::bind(&periodic_test::callback, this)), timers::exceptions::time_period_is_negative);
    EXPECT_THROW(m_periodic_timer.start(3s, nullptr), timers::exceptions::callback_not_callable);
}

TEST_F(periodic_test, stop)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_periodic_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(1s);

    EXPECT_NO_THROW(m_periodic_timer.stop());
    EXPECT_TRUE(m_periodic_timer.m_terminated);

    for (size_t i = 0; i < 100; ++i)
    {
        EXPECT_NO_THROW(m_periodic_timer.stop());
    }
}
}
