#pragma once

namespace burda
{
namespace timers
{
/// Helper class that enables default construction and disables copy and move operations
struct disable_copy_and_move
{
    disable_copy_and_move() = default;

    disable_copy_and_move(const disable_copy_and_move &) = delete;
    disable_copy_and_move(disable_copy_and_move &&) = delete;
    disable_copy_and_move & operator=(const disable_copy_and_move &) = delete;
    disable_copy_and_move & operator=(disable_copy_and_move &&) = delete;
};
}
}
