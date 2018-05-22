#include "test_utils.h"

namespace burda
{
namespace timers
{
namespace testing
{
void check_if_mutex_is_owned(std::mutex & mutex, const bool should_be_owned)
{
    const auto is_mutex_owned = !mutex.try_lock();
    EXPECT_EQ(is_mutex_owned, should_be_owned);

    if (!is_mutex_owned)
    {
        mutex.unlock();
    }
}
}
}
}
