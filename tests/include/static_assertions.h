#pragma once

namespace burda
{
namespace timers
{
namespace testing
{
template <typename class_type>
void assert_properties()
{
    static_assert(std::is_default_constructible<class_type>::value, "class is not default constructible (should be)");

    static_assert(!std::is_move_constructible<class_type>::value, "class is move constructible (shouldn't be)");
    static_assert(!std::is_move_assignable<class_type>::value, "blocking timer is move assignable (shouldn't be)");

    static_assert(!std::is_copy_constructible<class_type>::value, "blocking timer is copy constructible (shouldn't be)");
    static_assert(!std::is_copy_assignable<class_type>::value, "blocking timer is copy assignable (shouldn't be)");
}
}
}
}
