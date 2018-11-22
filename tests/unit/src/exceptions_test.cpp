#include <gtest/gtest.h>

#include <test_utils/lifetime_assertions.hpp>
#include <timers/exceptions.hpp>

namespace
{
namespace test_utils = burda::test_utils;
namespace timers = burda::timers;

TEST(exceptions_construction_destruction, construction_destruction)
{
    test_utils::assert_construction_and_destruction<timers::exceptions::callback_not_callable>();
    test_utils::assert_construction_and_destruction<timers::exceptions::time_period_is_invalid>();
    test_utils::assert_construction_and_destruction<timers::exceptions::time_period_is_zero>();
    test_utils::assert_construction_and_destruction<timers::exceptions::time_period_is_negative>();
}
}

