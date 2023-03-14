#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <string>

class GarbageBag
{
private:
    int weight;

public:
    explicit GarbageBag(const int weight)
    {
        this->weight = weight;
    }

    auto get_weight()
    {
        return this->weight;
    }
};

const auto BIN_WEIGHT_LIMIT = 100;

const auto GARBAGE_BAG_COUNT = 11;

using GarbageBags = std::array<GarbageBag, GARBAGE_BAG_COUNT>;

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

class Solution
{
private:
    GarbageBags bags;

public:
    Solution() : bags(GARBAGE_BAGS) {}

    auto swap_garbage_bags(int index1, int index2)
    {
        std::swap((this->bags)[index1], (this->bags)[index2]);
    }

    auto get_filled_bin_count()
    {
        auto binCount = 1;
        auto weightInLastBin = 0;

        for (auto &bag : this->bags)
        {
            auto bag_weight = bag.get_weight();

            if ((weightInLastBin + bag_weight) > BIN_WEIGHT_LIMIT)
            {
                binCount++;
                weightInLastBin = bag_weight;
            }
            else
            {
                weightInLastBin += bag_weight;
            }
        }

        return binCount;
    }

    auto to_string()
    {
        auto str = std::string{"Solution("};

        str += std::to_string(this->get_filled_bin_count());
        str += "){";

        auto first_bag = true;
        for (auto &bag : this->bags)
        {
            if (!first_bag)
            {
                str += ", ";
            }
            else
            {
                first_bag = false;
            }

            str += std::to_string(bag.get_weight());
        }

        str.push_back('}');

        return str;
    }
};

std::ostream &operator<<(std::ostream &o, Solution solution)
{
    o << solution.to_string().c_str();
    return o;
}

auto range(int len)
{
    auto v = std::vector<int>{};

    for (int i = 0; i < len; i++)
    {
        v.push_back(i);
    }

    return v;
}

std::random_device rd;
std::mt19937 rgen(rd());

class SolutionFactory
{
private:
    auto swap_random_adjacent_bag_pair(Solution solution)
    {
        std::uniform_int_distribution<int> distr(0, GARBAGE_BAG_COUNT - 1);

        auto random_index = distr(rgen);
        auto next_index = (random_index + 1) % GARBAGE_BAG_COUNT;

        solution.swap_garbage_bags(random_index, next_index);

        return solution;
    }

public:
    auto generate_hillclimbing_solution()
    {
        auto best_solution = Solution{};

        for (auto _ : range(1000))
        {
            auto new_solution = swap_random_adjacent_bag_pair(best_solution);

            if (new_solution.get_filled_bin_count() <= best_solution.get_filled_bin_count())
            {
                best_solution = new_solution;
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
