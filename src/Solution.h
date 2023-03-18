#include "GarbageBag.h"
#include <iostream>
#include <vector>

using GarbageBags = std::vector<GarbageBag>;

class Solution {
private:
    const int bin_weight_limit;
    GarbageBags garbage_bags;

public:
    Solution(
        int bin_weight_limit,
        GarbageBags garbage_bags)
        : bin_weight_limit(bin_weight_limit),
          garbage_bags(garbage_bags) {
    }

    auto get_garbage_bags() {
        return this->garbage_bags;
    }

    auto swap_garbage_bags(int index1, int index2) {
        std::swap((this->garbage_bags)[index1], (this->garbage_bags)[index2]);
    }

    auto get_filled_bin_count() {
        auto binCount = 1;
        auto weightInLastBin = 0;

        for (auto &bag : this->garbage_bags) {
            auto bag_weight = bag.get_weight();

            if ((weightInLastBin + bag_weight) > this->bin_weight_limit) {
                binCount++;
                weightInLastBin = bag_weight;
            } else {
                weightInLastBin += bag_weight;
            }
        }

        return binCount;
    }

    auto to_string() {
        auto str = std::string{"Solution(filled bins: "};

        str += std::to_string(this->get_filled_bin_count());
        str += "; garbage bags: ";

        auto first_bag = true;
        for (auto &bag : this->garbage_bags) {
            if (!first_bag) {
                str += ", ";
            } else {
                first_bag = false;
            }

            str += std::to_string(bag.get_weight());
        }

        str += ")";

        return str;
    }

    void operator=(Solution solution) {
        this->garbage_bags = solution.get_garbage_bags();
    }
};

std::ostream &operator<<(std::ostream &o, Solution solution) {
    o << solution.to_string().c_str();
    return o;
}
