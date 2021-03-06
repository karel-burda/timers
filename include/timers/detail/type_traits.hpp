#pragma once

#include <type_traits>

namespace burda
{
namespace timers
{
namespace detail
{
namespace traits
{
/// Helper structure to deduce return type of given method
/// Credit goes to the Piotr Skotnicki
/// @see https://stackoverflow.com/questions/26107041/how-can-i-determine-the-return-type-of-a-c11-member-function
template <typename T>
struct return_type_templated;

template <typename R, typename C, typename ... Args>
struct return_type_templated<R(C::*)(Args...)>
{
    using type = R;
};

template <typename T>
using return_type = typename return_type_templated<T>::type;
}
}
}
}
