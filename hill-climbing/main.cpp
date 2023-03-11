#include <iostream>
#include <array>
#include <vector>

const auto BIN_WEIGHT_LIMIT = 100;

class GarbageBag {
private:
    int weight;

public:
    explicit GarbageBag(const int weight) {
        this->weight = weight;
    }

    auto getWeight() const {
        return this->weight;
    }
};

const auto GARBAGE_BAGS = std::array{
        GarbageBag(50),
        GarbageBag(20),
        GarbageBag(40),
        GarbageBag(70),
        GarbageBag(10),
        GarbageBag(30),
        GarbageBag(80)
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
