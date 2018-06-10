#pragma once

namespace burda
{
namespace timers
{
namespace detail
{
/// Helper class that enables default construction and disables copy operations
struct disable_copy
{
    disable_copy() = default;

    disable_copy(const disable_copy &) = delete;
    disable_copy(disable_copy &&) = delete;
};
}
}
}
