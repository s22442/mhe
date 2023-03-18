#include "../Solution.h"
#include "../utils.h"
#include <random>
#include <vector>

const auto BIN_WEIGHT_LIMIT = 100;

// best possible bin count: 8
const auto GARBAGE_BAGS = std::vector<GarbageBag>{
    GarbageBag(48),
    GarbageBag(30),
    GarbageBag(36),
    GarbageBag(19),
    GarbageBag(36),
    GarbageBag(27),
    GarbageBag(42),
    GarbageBag(42),
    GarbageBag(36),
    GarbageBag(24),
    GarbageBag(30),
    GarbageBag(48),
    GarbageBag(30),
    GarbageBag(36),
    GarbageBag(19),
    GarbageBag(36),
    GarbageBag(27),
    GarbageBag(42),
    GarbageBag(42),
    GarbageBag(36),
    GarbageBag(24),
    GarbageBag(30),
};

std::random_device rd;
std::mt19937 rgen(rd());
const auto garbage_bag_count = GARBAGE_BAGS.size();
std::uniform_int_distribution<int> distr(0, garbage_bag_count - 1);

class SolutionFactory {
private:
    auto generate_random_bag_index() {
        return distr(rgen);
    }

    auto swap_random_adjacent_bag_pair(Solution solution) {
        auto random_index = this->generate_random_bag_index();
        auto next_index = (random_index + 1) % garbage_bag_count;

        solution.swap_garbage_bags(random_index, next_index);

        return solution;
    }

    auto swap_random_bag_with_best_neighbor(Solution solution) {
        auto random_index = this->generate_random_bag_index();

        auto best_solution = solution;

        for (auto i : range(garbage_bag_count)) {
            if (i == random_index) {
                continue;
            }

            auto new_solution = solution;
            new_solution.swap_garbage_bags(random_index, i);

            if (new_solution.get_filled_bin_count() < best_solution.get_filled_bin_count()) {
                best_solution = new_solution;
            }
        }

        return best_solution;
    }

public:
    auto generate_random_hillclimbing_solution() {
        auto best_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};

        while (true) {
            auto new_solution = swap_random_adjacent_bag_pair(best_solution);

            if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count()) {
                best_solution = new_solution;
            } else {
                break;
            }
        }

        return best_solution;
    }

    auto generate_best_neighbor_hillclimbing_solution() {
        auto best_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};

        while (true) {
            auto new_solution = swap_random_bag_with_best_neighbor(best_solution);

            if (new_solution.get_filled_bin_count() < best_solution.get_filled_bin_count()) {
                best_solution = new_solution;
            } else {
                break;
            }
        }

        return best_solution;
    }
};

int main() {
    auto solution_factory = SolutionFactory{};

    std::cout
        << "Random hill climbing solution: "
        << std::endl
        << solution_factory.generate_random_hillclimbing_solution()
        << std::endl;

    std::cout << std::endl;

    std::cout
        << "Best neighbor hill climbing solution: "
        << std::endl
        << solution_factory.generate_best_neighbor_hillclimbing_solution()
        << std::endl;

    return 0;
}
