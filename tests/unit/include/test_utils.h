#pragma once

#include <mutex>

#include <gtest/gtest.h>

namespace burda
{
namespace timers
{
namespace testing
{
template <typename class_type>
void assert_construction_and_destruction()
{
    ASSERT_NO_THROW(class_type instance);

    class_type instance;
    EXPECT_NO_THROW(instance.~class_type());
}

void check_if_mutex_is_owned(std::mutex & mutex, bool should_be_owned);
}
}
}
