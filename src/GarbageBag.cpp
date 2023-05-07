
#include "GarbageBag.h"

auto operator<(const GarbageBag &bag_a, const GarbageBag &bag_b) -> bool {
    return bag_a._weight < bag_b._weight;
}
