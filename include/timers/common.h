#pragma once

#include <chrono>

// TODO
#define DISABLE_COPY_OPERATIONS()
#define DISABLE_MOVE_OPERATIONS()

namespace burda
{
namespace timers
{
using time_interval = std::chrono::duration<double>;
using timers_callback = std::function<void()>;

class no_copy_and_move_operations
{
public:
    no_copy_and_move_operations() = default;
    ~no_copy_and_move_operations() = default;
    no_copy_and_move_operations(const no_copy_and_move_operations &) = delete;
    no_copy_and_move_operations(no_copy_and_move_operations &&) = delete;
    no_copy_and_move_operations & operator=(const no_copy_and_move_operations &) = delete;
    no_copy_and_move_operations & operator=(no_copy_and_move_operations &&) = delete;
};
}
}
