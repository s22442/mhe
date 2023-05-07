#include "GarbageBag.h"
#include "utils.h"
#include <iostream>
#include <random>
#include <vector>

#ifndef SOLUTION_H
#define SOLUTION_H

using GarbageBags = std::vector<GarbageBag>;

class Solution {
private:
    const int bin_weight_limit;
    GarbageBags garbage_bags;

    auto generate_random_bag_index() -> int;

public:
    Solution(
        int bin_weight_limit,
        GarbageBags garbage_bags);

    inline auto get_garbage_bags() -> GarbageBags {
        return this->garbage_bags;
    }

    auto swap_garbage_bags(int index1, int index2) -> void;

    auto generate_neighbors() -> std::vector<Solution>;

    auto generate_random_neighbor() -> Solution;

    auto get_bins() -> std::vector<GarbageBags>;

    auto get_filled_bin_count() -> int;

    auto to_string() -> std::string;

    auto operator=(Solution solution) -> void;
};

std::ostream &operator<<(std::ostream &o, Solution solution);

#endif // SOLUTION_H
