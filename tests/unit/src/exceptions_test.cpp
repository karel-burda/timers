#include <gtest/gtest.h>

#include <test_utils/lifetime.hpp>
#include <timers/exceptions.hpp>

namespace
{
namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

TEST(exceptions_construction_destruction, construction_destruction)
{
    test_utils::lifetime::assert_construction_and_destruction<timers::exceptions::callback_not_callable>();
    test_utils::lifetime::assert_construction_and_destruction<timers::exceptions::time_period_is_invalid>();
    test_utils::lifetime::assert_construction_and_destruction<timers::exceptions::time_period_is_zero>();
    test_utils::lifetime::assert_construction_and_destruction<timers::exceptions::time_period_is_negative>();
}
}

