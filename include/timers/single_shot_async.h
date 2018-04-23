#pragma once

#include "timers/async.h"
#include "timers/single_shot.h"

namespace burda
{
namespace timers
{
class single_shot_async : public single_shot, async<single_shot>
{
};
}
}
