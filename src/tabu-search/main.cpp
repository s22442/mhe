#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
#include <list>
#include <set>
#include <vector>

const auto BIN_WEIGHT_LIMIT = 100;

// best possible bin count: 4
const auto GARBAGE_BAGS = GarbageBags{
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

auto is_tabu_infinite(int tabu_size) -> bool {
    return tabu_size <= 0;
}

class SolutionFactory {
public:
    auto generate_tabu_search_solution(int tabu_size, bool backtracking = false) {
        auto current_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};
        auto best_solution = current_solution;

        auto tabu = std::set<GarbageBags>{current_solution.get_garbage_bags()};
        auto previous_solutions = std::vector<Solution>{current_solution};

        while (is_tabu_infinite(tabu_size) || tabu.size() < tabu_size) {
            auto neighbors = current_solution.generate_neighbors();

            neighbors.erase(
                std::remove_if(
                    neighbors.begin(),
                    neighbors.end(),
                    [&](Solution neighbor) { return tabu.contains(neighbor.get_garbage_bags()); }),
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

            tabu.insert(new_solution.get_garbage_bags());
            previous_solutions.push_back(new_solution);
            current_solution = std::move(new_solution);
        }

        return best_solution;
    }
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    auto tabu_size = -1;

    if (argc >= 2) {
        if (std::string{argv[1]} == "help") {
            std::cout
                << "Args:"
                << std::endl
                << "1. Size of tabu. Tabu not greater than 0 means infinite tabu."
                << std::endl
                << "   Default: " << tabu_size
                << std::endl;

            return 0;
        } else {
            tabu_size = std::stoi(argv[1]);
        }
    }

    if (is_tabu_infinite(tabu_size)) {
        std::cout
            << "Infinite tabu takes a while. Sit back and relax!"
            << std::endl
            << std::endl;
    }

    std::cout
        << "Tabu search solution:"
        << std::endl
        << solution_factory.generate_tabu_search_solution(tabu_size)
        << std::endl;

    std::cout << std::endl;

    std::cout
        << "Tabu search with backtracking solution:"
        << std::endl
        << solution_factory.generate_tabu_search_solution(tabu_size, true)
        << std::endl;

    return 0;
}
