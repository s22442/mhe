#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
#include <list>
#include <map>
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

public:
    auto generate_simulated_annealing_solution(int iteration_count, std::function<double(int)> temperature_cb) {
        auto current_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};
        auto best_solution = current_solution;

        for (auto i : range(iteration_count)) {
            auto new_solution = this->swap_random_adjacent_bag_pair(current_solution);
            if (new_solution.get_filled_bin_count() <= current_solution.get_filled_bin_count()) {
                current_solution = new_solution;
                if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count()) {
                    best_solution = current_solution;
                }
            } else {
                std::uniform_real_distribution<double> distr(0.0, 1.0);
                if (
                    distr(rgen) < std::exp(
                                      -std::abs(
                                          new_solution.get_filled_bin_count() - current_solution.get_filled_bin_count()) /
                                      temperature_cb(i))) {
                    current_solution = new_solution;
                }
            }
        }

        return best_solution;
    }
};

auto TEMPERATURE_CB_MAP = std::map<int, std::function<double(int)>>{
    {1, [](int k) { return 1.0 / k; }},
    {2, [](int k) { return 1.0 / std::log10(k); }},
    {3, [](int k) { return std::pow(0.5, k); }},
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    auto iteration_count = 1000;
    auto temperature_cb_key = 1;

    if (argc >= 2) {
        if (std::string{argv[1]} == "help") {
            std::cout
                << "Args:"
                << std::endl
                << "1. Iteration count."
                << std::endl
                << "   Default: " << iteration_count
                << std::endl
                << "2. Algorithm."
                << std::endl
                << "   - 1 -> T ~== 1 / k"
                << std::endl
                << "   - 2 -> T ~== 1 / log(k)"
                << std::endl
                << "   - 3 -> T ~== 1 / a^k"
                << std::endl
                << "   Default: " << temperature_cb_key
                << std::endl;

            return 0;
        } else {
            iteration_count = std::stoi(argv[1]);

            if (argc >= 3) {
                auto new_temperature_cb_key = std::stoi(argv[2]);

                if (TEMPERATURE_CB_MAP.contains(new_temperature_cb_key)) {
                    temperature_cb_key = new_temperature_cb_key;
                }
            }
        }
    }

    std::cout
        << "Simulated annealing solution:"
        << std::endl
        << solution_factory.generate_simulated_annealing_solution(
               iteration_count,
               TEMPERATURE_CB_MAP[temperature_cb_key])
        << std::endl;

    return 0;
}
