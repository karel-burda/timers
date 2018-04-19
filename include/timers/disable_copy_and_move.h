#pragma once

namespace burda
{
namespace timers
{
class disable_copy_and_move
{
public:
    disable_copy_and_move() = default;
    virtual ~disable_copy_and_move() = default;
    disable_copy_and_move(const disable_copy_and_move &) = delete;
    disable_copy_and_move(disable_copy_and_move &&) = delete;
    disable_copy_and_move & operator=(const disable_copy_and_move &) = delete;
    disable_copy_and_move & operator=(disable_copy_and_move &&) = delete;
};
}
}
