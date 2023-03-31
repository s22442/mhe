#include "GarbageBag.h"
#include "utils.h"
#include <iostream>
#include <random>
#include <vector>

#ifndef SOLUTION_H
#define SOLUTION_H

using GarbageBags = std::vector<GarbageBag>;

auto _solution_rd = std::random_device{};
auto _solution_rgen = std::mt19937{_solution_rd()};

class Solution {
private:
    const int bin_weight_limit;
    GarbageBags garbage_bags;

    auto generate_random_bag_index() {
        auto distr = std::uniform_int_distribution<int>{
            0,
            (int)this->garbage_bags.size() - 1};

        return distr(_solution_rgen);
    }

public:
    Solution(
        int bin_weight_limit,
        GarbageBags garbage_bags)
        : bin_weight_limit(bin_weight_limit),
          garbage_bags(garbage_bags) {
    }

    auto get_garbage_bags() -> GarbageBags {
        return this->garbage_bags;
    }

    auto swap_garbage_bags(int index1, int index2) -> void {
        std::swap((this->garbage_bags)[index1], (this->garbage_bags)[index2]);
    }

    auto generate_neighbors() -> std::vector<Solution> {
        auto neighbors = std::vector<Solution>{};

        auto garbage_bags_size = this->garbage_bags.size();
        for (auto i : range(garbage_bags_size)) {
            auto neighbor = *this;
            neighbor.swap_garbage_bags(i, (i + 1) % garbage_bags_size);
            neighbors.push_back(neighbor);
        }

        return neighbors;
    }

    auto generate_random_neighbor() {
        auto neighbor = *this;

        auto random_index = this->generate_random_bag_index();
        auto next_index = (random_index + 1) % this->garbage_bags.size();

        neighbor.swap_garbage_bags(random_index, next_index);

        return neighbor;
    }

    auto get_filled_bin_count() -> int {
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

    auto to_string() -> std::string {
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

    auto operator=(Solution solution) -> void {
        this->garbage_bags = solution.get_garbage_bags();
    }
};

std::ostream &operator<<(std::ostream &o, Solution solution) {
    o << solution.to_string().c_str();
    return o;
}

#endif // SOLUTION_H
