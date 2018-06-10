#pragma once

namespace burda
{
namespace timers
{
namespace detail
{
/// Helper class that enables default construction and disables move operations
struct disable_move
{
    disable_move() = default;

    disable_move & operator=(const disable_move &) = delete;
    disable_move & operator=(disable_move &&) = delete;
};
}
}
}
