#include <gtest/gtest.h>

#include <timers/exceptions.h>

#include "test_utils.h"

namespace
{
namespace timers = burda::timers;

TEST(exceptions_construction_destruction, construction_destruction)
{
    timers::testing::assert_construction_and_destruction<timers::exceptions::callback_not_callable>();
    timers::testing::assert_construction_and_destruction<timers::exceptions::time_period_is_invalid>();
    timers::testing::assert_construction_and_destruction<timers::exceptions::time_period_is_zero>();
    timers::testing::assert_construction_and_destruction<timers::exceptions::time_period_is_negative>();
}
}

