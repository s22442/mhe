#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
#include <list>
#include <random>
#include <set>
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

class SolutionFactory {
public:
    auto generate_tabu_search_solution(int tabu_size) {
        auto current_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};
        auto best_solution = current_solution;

        auto tabu = std::set<GarbageBags>{current_solution.get_garbage_bags()};

        while (tabu_size <= 0 || tabu.size() < tabu_size) {
            auto neighbors = current_solution.generate_neighbors();

            neighbors.erase(
                std::remove_if(
                    neighbors.begin(),
                    neighbors.end(),
                    [&](Solution x) { return tabu.contains(x.get_garbage_bags()); }),
                neighbors.end());

            if (neighbors.size() == 0) {
                std::cout << "Ate my tail..." << std::endl;
                return best_solution;
            }

            auto new_solution = *std::max_element(neighbors.begin(), neighbors.end(), [](auto a, auto b) {
                return a.get_filled_bin_count() > b.get_filled_bin_count();
            });

            if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count()) {
                best_solution = new_solution;
            }

            tabu.insert(new_solution.get_garbage_bags());
            current_solution = std::move(new_solution);
        }

        return best_solution;
    }

    auto generate_tabu_search_with_backtracking_solution(int tabu_size) {
        auto best_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};

        // TODO

        return best_solution;
    }
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    // tabu not greater than 0 means infinite tabu
    // you may also want to comment out some bags before trying infinite tabu
    auto tabu_size = argc >= 2 ? std::stoi(argv[1]) : -1;

    std::cout
        << "Tabu search solution:"
        << std::endl
        << solution_factory.generate_tabu_search_solution(tabu_size)
        << std::endl;

    std::cout << std::endl;

    std::cout
        << "Tabu search with backtracking solution:"
        << std::endl
        << solution_factory.generate_tabu_search_with_backtracking_solution(tabu_size)
        << std::endl;

    return 0;
}
