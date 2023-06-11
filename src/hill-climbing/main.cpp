#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
#include <vector>

const auto BIN_WEIGHT_LIMIT = 100;
const auto GARBAGE_BAGS = load_garbage_bags();

class SolutionFactory {
private:
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
            auto new_solution = best_solution.generate_random_neighbor();

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
