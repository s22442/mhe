#ifndef GARBAGE_BAG_H
#define GARBAGE_BAG_H

class GarbageBag {
public:
    int _weight;

public:
    explicit GarbageBag(const int weight) {
        this->_weight = weight;
    }

    auto get_weight() -> int {
        return this->_weight;
    }
};

auto operator<(const GarbageBag &bag_a, const GarbageBag &bag_b) -> bool {
    return bag_a._weight < bag_b._weight;
}

#endif // GARBAGE_BAG_H
