#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
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

const auto garbage_bag_count = GARBAGE_BAGS.size();

std::random_device rd;
std::mt19937 rgen(rd());

class SolutionFactory {
private:
    auto generate_random_bag_index() {
        std::uniform_int_distribution<int> distr(0, garbage_bag_count - 1);
        return distr(rgen);
    }

    auto swap_random_adjacent_bag_pair(Solution solution) {
        auto random_index = this->generate_random_bag_index();
        auto next_index = (random_index + 1) % garbage_bag_count;

        solution.swap_garbage_bags(random_index, next_index);

        return solution;
    }

    auto get_best_neighbor(Solution solution) {
        auto neighbors = solution.generate_neighbors();

        return *std::max_element(neighbors.begin(), neighbors.end(), [](auto a, auto b) {
            return a.get_filled_bin_count() > b.get_filled_bin_count();
        });
    }

public:
    auto generate_random_hillclimbing_solution() {
        auto best_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};

        while (true) {
            auto new_solution = swap_random_adjacent_bag_pair(best_solution);

            if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count()) {
                best_solution = std::move(new_solution);
            } else {
                break;
            }
        }

        return best_solution;
    }

    auto generate_deterministic_hillclimbing_solution() {
        auto best_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};

        while (true) {
            auto new_solution = get_best_neighbor(best_solution);

            if (new_solution.get_filled_bin_count() < best_solution.get_filled_bin_count()) {
                best_solution = std::move(new_solution);
            } else {
                break;
            }
        }

        return best_solution;
    }
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    if (argc >= 2) {
        std::cout
            << "No args allowed!"
            << std::endl;

        return 0;
    }

    std::cout
        << "Random hill climbing solution:"
        << std::endl
        << solution_factory.generate_random_hillclimbing_solution()
        << std::endl;

    std::cout << std::endl;

    std::cout
        << "Deterministic hill climbing solution:"
        << std::endl
        << solution_factory.generate_deterministic_hillclimbing_solution()
        << std::endl;

    return 0;
}
