#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic_async.h>
#include <timers/exceptions.h>

#include "static_assertions.h"
#include "test_utils.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class periodic_async_test : public ::testing::Test
{
protected:
    timers::periodic_async m_periodic_async_timer;
};

TEST_F(periodic_async_test, static_assertions)
{
    burda::timers::testing::assert_properties<decltype(m_periodic_async_timer)>();

    SUCCEED();
}

TEST(periodic_async_construction_destruction, basic_construction_destruction)
{
    burda::timers::testing::assert_construction_and_destruction<burda::timers::periodic_async>();
}
}
