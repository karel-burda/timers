#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic.h>

#include "common.h"

namespace
{
using namespace std::chrono_literals;

class periodic_test : public ::testing::Test
{
protected:
    void callback()
    {
        ++m_counter;
    }

    unsigned char m_counter = 0;
    burda::timers::periodic m_periodic_timer;
};

TEST(periodic_construction_destruction, basic_construction_destruction)
{
    ASSERT_NO_THROW(burda::timers::periodic periodic_timer);

    burda::timers::periodic periodic_timer;
    EXPECT_NO_THROW(periodic_timer.~periodic_timer());
}

TEST_F(periodic_test, callback)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_periodic_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(6s);

    EXPECT_GE(m_counter, 5);

    m_periodic_timer.stop();
}

TEST_F(periodic_test, stop)
{
    auto caller = std::async(std::launch::async, [this]()
    {
        m_periodic_timer.start(1s, std::bind(&periodic_test::callback, this));
    });

    std::this_thread::sleep_for(1s);

    EXPECT_NO_THROW(m_periodic_timer.stop());
    EXPECT_TRUE(m_periodic_timer.m_blocking_timer.m_terminated);

    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_NO_THROW(m_periodic_timer.stop());
    }
}
}
