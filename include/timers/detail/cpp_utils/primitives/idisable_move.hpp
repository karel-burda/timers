#pragma once

/// Origin: https://github.com/karel-burda/cpp-utils
/// Version: 1.1.1

namespace burda
{
namespace cpp_utils
{
namespace primitives
{
/// Helper class that enables default construction and disables move operations
/// Note that even this class is actually std::move_constructible, because the r-value might be transformed
/// to the l-value and copy construction might be made.
/// Recommended way of inheriting from this base class is private
struct idisable_move
{
    idisable_move() = default;
    ~idisable_move() = default;

    idisable_move(const idisable_move &) = default;
    idisable_move & operator=(const idisable_move &) = default;

    idisable_move(idisable_move &&) = delete;
    idisable_move & operator=(idisable_move &&) = delete;
};
}
}
}
