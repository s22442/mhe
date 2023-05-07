#include "Solution.h"
#include "GarbageBag.h"
#include "utils.h"
#include <iostream>
#include <random>
#include <vector>

using GarbageBags = std::vector<GarbageBag>;

auto _solution_rd = std::random_device{};
auto _solution_rgen = std::mt19937{_solution_rd()};

auto Solution::generate_random_bag_index() -> int {
    auto distr = std::uniform_int_distribution<int>{
        0,
        (int)this->garbage_bags.size() - 1};

    return distr(_solution_rgen);
}

Solution::Solution(
    int bin_weight_limit,
    GarbageBags garbage_bags)
    : bin_weight_limit(bin_weight_limit),
      garbage_bags(garbage_bags) {
}

auto Solution::swap_garbage_bags(int index1, int index2) -> void {
    std::swap((this->garbage_bags)[index1], (this->garbage_bags)[index2]);
}

auto Solution::generate_neighbors() -> std::vector<Solution> {
    auto neighbors = std::vector<Solution>{};

    auto garbage_bags_size = this->garbage_bags.size();
    for (auto i : range(garbage_bags_size)) {
        auto neighbor = *this;
        neighbor.swap_garbage_bags(i, (i + 1) % garbage_bags_size);
        neighbors.push_back(neighbor);
    }

    return neighbors;
}

auto Solution::generate_random_neighbor() -> Solution {
    auto neighbor = *this;

    auto random_index = this->generate_random_bag_index();
    auto next_index = (random_index + 1) % this->garbage_bags.size();

    neighbor.swap_garbage_bags(random_index, next_index);

    return neighbor;
}

auto Solution::get_bins() -> std::vector<GarbageBags> {
    auto bins = std::vector<GarbageBags>{{}};

    auto weight_in_last_bin = 0;

    for (auto bag : this->garbage_bags) {
        auto bag_weight = bag.get_weight();

        if ((weight_in_last_bin + bag_weight) > this->bin_weight_limit) {
            bins.push_back({std::move(bag)});
            weight_in_last_bin = bag_weight;
        } else {
            bins.back().push_back(std::move(bag));
            weight_in_last_bin += bag_weight;
        }
    }

    return bins;
}

auto Solution::get_filled_bin_count() -> int {
    return this->get_bins().size();
}

auto Solution::to_string() -> std::string {
    auto str = std::string{"Solution(filled bins: "};

    str += std::to_string(this->get_filled_bin_count());
    str += "; garbage bags: ";

    auto first_bin = true;
    for (auto &bin : this->get_bins()) {
        if (!first_bin) {
            str += " | ";
        } else {
            first_bin = false;
        }

        auto first_bag = true;
        for (auto &bag : bin) {
            if (!first_bag) {
                str += ", ";
            } else {
                first_bag = false;
            }

            str += std::to_string(bag.get_weight());
        }
    }

    str += ")";

    return str;
}

auto Solution::operator=(Solution solution) -> void {
    this->garbage_bags = solution.get_garbage_bags();
}

std::ostream &operator<<(std::ostream &o, Solution solution) {
    o << solution.to_string().c_str();
    return o;
}
