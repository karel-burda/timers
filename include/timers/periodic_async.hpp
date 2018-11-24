#pragma once

#include "timers/async.hpp"
#include "timers/periodic.hpp"

namespace burda
{
namespace timers
{
class periodic_async : public async<periodic>
{
};
}
}
