#pragma once

#include "timers/async.h"
#include "timers/periodic.h"

namespace burda
{
namespace timers
{
class periodic_async : public async<periodic>
{
};
}
}
