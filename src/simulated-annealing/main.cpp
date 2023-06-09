#include "../Solution.h"
#include "../utils.h"
#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <vector>

const auto BIN_WEIGHT_LIMIT = 100;
const auto GARBAGE_BAGS = load_garbage_bags();

auto _sa_rd = std::random_device{};
auto _sa_rgen = std::mt19937{_sa_rd()};

class SolutionFactory {
public:
    auto generate_simulated_annealing_solution(int iteration_count, std::function<double(int)> temperature_cb) {
        auto current_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};
        auto best_solution = current_solution;

        for (auto i : range(iteration_count)) {
            auto new_solution = current_solution.generate_random_neighbor();
            if (new_solution.get_filled_bin_count() <= current_solution.get_filled_bin_count()) {
                current_solution = new_solution;
                if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count()) {
                    best_solution = current_solution;
                }
            } else {
                std::uniform_real_distribution<double> distr(0.0, 1.0);
                if (
                    distr(_sa_rgen) < std::exp(
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

    auto args = collect_args({
                                 {
                                     "Iteration count",
                                     "",
                                     {},
                                     1000,
                                 },
                                 {
                                     "Algorithm",
                                     "- 1 -> T ~== 1 / k"
                                     "\n   - 2 -> T ~== 1 / log(k)"
                                     "\n   - 3 -> T ~== 1 / a^k",
                                     {map_keys_to_set(TEMPERATURE_CB_MAP)},
                                     1,
                                 },
                             },
                             argc, argv);

    if (!args.size()) {
        return 0;
    }

    std::cout
        << "Simulated annealing solution:"
        << std::endl
        << solution_factory.generate_simulated_annealing_solution(
               args[0],
               TEMPERATURE_CB_MAP[args[1]])
        << std::endl;

    return 0;
}
