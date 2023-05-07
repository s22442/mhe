#ifndef GARBAGE_BAG_H
#define GARBAGE_BAG_H

class GarbageBag {
public:
    int _weight;

public:
    inline explicit GarbageBag(const int weight) {
        this->_weight = weight;
    }

    inline auto get_weight() -> int {
        return this->_weight;
    }
};

auto operator<(const GarbageBag &bag_a, const GarbageBag &bag_b) -> bool;

#endif // GARBAGE_BAG_H
