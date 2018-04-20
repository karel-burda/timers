#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic.h>

#include "static_assertions.h"
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

TEST(blocking_test_static, static_assertions)
{
    assert_properties<timers::periodic>();

    SUCCEED();
}

TEST(periodic_construction_destruction, basic_construction_destruction)
{
    ASSERT_NO_THROW(timers::periodic periodic_timer);

    timers::periodic periodic_timer;
    EXPECT_NO_THROW(periodic_timer.timers::periodic::~periodic());
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
    EXPECT_TRUE(m_periodic_timer.m_terminated);

    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_NO_THROW(m_periodic_timer.stop());
    }
}
}
