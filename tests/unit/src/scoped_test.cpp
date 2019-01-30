#include <thread>

#include <gtest/gtest.h>

#include <test_utils/lifetime.hpp>
#include <test_utils/statics.hpp>
#include <timers/periodic.hpp>
#include <timers/periodic_async.hpp>
#include <timers/scoped.hpp>
#include <timers/single_shot.hpp>
#include <timers/single_shot_async.hpp>

namespace
{
using namespace std::chrono_literals;

namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

class scoped_test : public ::testing::Test
{
protected:
    timers::scoped<timers::periodic_async> m_periodic_async;
};

TEST(scoped_construction_destruction, construction_destruction)
{
    test_utils::lifetime::assert_construction_and_destruction<timers::scoped<timers::single_shot>>();
    test_utils::lifetime::assert_construction_and_destruction<timers::scoped<timers::single_shot_async>>();

    test_utils::lifetime::assert_construction_and_destruction<timers::scoped<timers::periodic>>();
    test_utils::lifetime::assert_construction_and_destruction<timers::scoped<timers::periodic_async>>();
}

TEST_F(scoped_test, static_assertions)
{
    test_utils::statics::assert_default_constructibility<decltype(m_periodic_async), true>();
    test_utils::statics::assert_copy_constructibility<decltype(m_periodic_async), false>();
    test_utils::statics::assert_move_constructibility<decltype(m_periodic_async), false>();
}

TEST_F(scoped_test, operators)
{
    EXPECT_NO_THROW(m_periodic_async.operator*());
    EXPECT_NO_THROW(m_periodic_async.operator->());
}

TEST_F(scoped_test, start)
{
    bool callback_called = false;

    m_periodic_async->start(1s, [&callback_called]() { callback_called = true; });

    std::this_thread::sleep_for(3s);

    EXPECT_TRUE(callback_called);
}
}
