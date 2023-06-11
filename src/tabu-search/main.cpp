#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <set>
#include <vector>

const auto BIN_WEIGHT_LIMIT = 100;
const auto GARBAGE_BAGS = load_garbage_bags();

auto is_tabu_infinite(int tabu_size) -> bool {
    return tabu_size <= 0;
}

class SolutionFactory {
public:
    auto generate_tabu_search_solution(int tabu_size, int iteration_count, bool backtracking = false) {
        auto current_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};
        auto best_solution = current_solution;

        auto tabu = std::vector<Solution>{current_solution};
        auto previous_solutions = std::vector<Solution>{current_solution};

        for (auto _ : range(iteration_count)) {
            auto neighbors = current_solution.generate_neighbors();

            neighbors.erase(
                std::remove_if(
                    neighbors.begin(),
                    neighbors.end(),
                    [&](Solution neighbor) {
                        for (auto tabu_solution : tabu) {
                            auto tabu_bags = tabu_solution.get_garbage_bags();
                            auto neighbor_bags = neighbor.get_garbage_bags();

                            auto found = true;
                            for (auto i : range(tabu_bags.size())) {
                                if (tabu_bags[i].get_weight() != neighbor_bags[i].get_weight()) {
                                    found = false;
                                    break;
                                }
                            }

                            if (found) {
                                return true;
                            }
                        }

                        return false;
                    }),
                neighbors.end());

            if (neighbors.size() == 0) {
                if (!backtracking || previous_solutions.size() == 0) {
                    return best_solution;
                }

                current_solution = previous_solutions.back();
                previous_solutions.pop_back();

                continue;
            }

            auto new_solution = *std::max_element(
                neighbors.begin(),
                neighbors.end(),
                [](auto a, auto b) {
                    return a.get_filled_bin_count() > b.get_filled_bin_count();
                });

            if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count()) {
                best_solution = new_solution;
            }

            tabu.push_back(new_solution);
            previous_solutions.push_back(new_solution);
            current_solution = std::move(new_solution);

            if (!is_tabu_infinite(tabu_size) && tabu.size() > tabu_size) {
                tabu.erase(tabu.begin());
            }
        }

        return best_solution;
    }
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    auto args = collect_args({
                                 {
                                     "Size of tabu",
                                     "Tabu not greater than 0 means infinite tabu",
                                     {},
                                     -1,
                                 },
                                 {
                                     "Iteration count",
                                     "",
                                     {},
                                     1000,
                                 },
                             },
                             argc, argv);

    if (!args.size()) {
        return 0;
    }

    auto tabu_size = args[0];
    auto iteration_count = args[1];

    if (is_tabu_infinite(tabu_size)) {
        std::cout
            << "Infinite tabu takes a while. Sit back and relax!"
            << std::endl
            << std::endl;
    }

    std::cout
        << "Tabu search solution:"
        << std::endl
        << solution_factory.generate_tabu_search_solution(tabu_size, iteration_count)
        << std::endl;

    std::cout << std::endl;

    std::cout
        << "Tabu search with backtracking solution:"
        << std::endl
        << solution_factory.generate_tabu_search_solution(tabu_size, iteration_count, true)
        << std::endl;

    return 0;
}
