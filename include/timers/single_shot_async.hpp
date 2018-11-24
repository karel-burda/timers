#pragma once

#include "timers/async.hpp"
#include "timers/single_shot.hpp"

namespace burda
{
namespace timers
{
class single_shot_async : public async<single_shot>
{
};
}
}
