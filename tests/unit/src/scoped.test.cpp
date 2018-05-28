#include <exception>
#include <functional>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic.h>
#include <timers/periodic_async.h>
#include <timers/single_shot.h>
#include <timers/single_shot_async.h>
#include <timers/scoped.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class scoped_test : public ::testing::Test
{
private:
    bool m_callback_called = false;

    timers::scoped<timers::periodic_async> m_periodic_async;
};

// TODO: Enable again after further investigation on other platforms
TEST(DISABLED_scoped_construction_destruction, construction_destruction)
{
    timers::testing::assert_construction_and_destruction<timers::scoped<timers::single_shot>>();
    timers::testing::assert_construction_and_destruction<timers::scoped<timers::single_shot_async>>();

    timers::testing::assert_construction_and_destruction<timers::scoped<timers::periodic>>();
    timers::testing::assert_construction_and_destruction<timers::scoped<timers::periodic_async>>();
}

TEST_F(scoped_test, static_assertions)
{
    timers::testing::assert_default_constructibility<decltype(m_periodic_async)>();
    timers::testing::assert_copy_constructibility<decltype(m_periodic_async)>();
    timers::testing::assert_move_constructibility<decltype(m_periodic_async)>();

    SUCCEED();
}

TEST_F(scoped_test, default_values)
{
    EXPECT_FALSE(m_periodic_async->m_async_task.valid());
    timers::testing::check_if_mutex_is_owned(m_periodic_async->m_async_protection, false);
    timers::testing::check_if_mutex_is_owned(m_periodic_async->m_cv_protection, false);
}

TEST_F(scoped_test, operators)
{
    EXPECT_NO_THROW(m_periodic_async.operator*());
    EXPECT_NO_THROW(m_periodic_async.operator->());
}

TEST_F(scoped_test, start)
{
    bool s = false;

    m_periodic_async->start(1s, [&s]() { s = true; });

    std::this_thread::sleep_for(5s);

    EXPECT_TRUE(s);
}
}
