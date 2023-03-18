#include <vector>
#include <random>
#include "../utils.h"
#include "../Solution.h"

const auto BIN_WEIGHT_LIMIT = 100;

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
};

std::random_device rd;
std::mt19937 rgen(rd());

const auto garbage_bag_count = GARBAGE_BAGS.size();

class SolutionFactory
{
private:
    auto swap_random_adjacent_bag_pair(Solution solution)
    {
        std::uniform_int_distribution<int> distr(0, garbage_bag_count - 1);

        auto random_index = distr(rgen);
        auto next_index = (random_index + 1) % garbage_bag_count;

        solution.swap_garbage_bags(random_index, next_index);

        return solution;
    }

public:
    auto generate_hillclimbing_solution()
    {
        auto best_solution = Solution{BIN_WEIGHT_LIMIT, GARBAGE_BAGS};

        while (true)
        {
            auto new_solution = swap_random_adjacent_bag_pair(best_solution);

            if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count())
            {
                best_solution = new_solution;
            }
            else
            {
                break;
            }
        }

        return best_solution;
    }
};

int main()
{
    auto solution_factory = SolutionFactory{};

    auto hillclimbing_solution = solution_factory.generate_hillclimbing_solution();

    std::cout << "Hill climbing solution: " << hillclimbing_solution << std::endl;

    return 0;
}
