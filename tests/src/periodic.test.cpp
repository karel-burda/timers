#include <chrono>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/periodic.h>

namespace
{
TEST(periodic, construction)
{
    ASSERT_NO_THROW(timers::periodic blocking_timer{ std::chrono::seconds{1} });
}
}
