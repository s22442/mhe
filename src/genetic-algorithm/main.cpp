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

using Population = std::vector<Solution>;
using SolutionPair = std::pair<Solution, Solution>;
using CrossoverCb = std::function<SolutionPair(Solution, Solution)>;
using MutationCb = std::function<Solution(Solution)>;
using EndingConditionCb = std::function<bool(Population, int)>;

auto _factory_rd = std::random_device{};
auto _factory_rgen = std::mt19937{_factory_rd()};

class SolutionFactory {
private:
    auto generate_random_solution() {
        auto bags = GARBAGE_BAGS;
        std::shuffle(bags.begin(), bags.end(), _factory_rgen);
        return Solution{BIN_WEIGHT_LIMIT, std::move(bags)};
    }

    auto generate_population(int population_size) {
        auto population = Population{};

        for (auto _ : range(population_size)) {
            population.push_back(this->generate_random_solution());
        }

        return population;
    }

    auto calculate_fitness(Solution &solution) {
        return 1.0 / (1 + solution.get_filled_bin_count());
    }

    auto select_parents(Population &population) {
        auto fitnesses = std::vector<int>{};

        for (auto i : range(population.size())) {
            fitnesses.push_back(this->calculate_fitness(population[i]));
        }

        auto parents = Population{};
        auto dist = std::uniform_int_distribution<int>{0, (int)population.size() - 1};

        for (auto i : range(population.size())) {
            auto index_a = dist(_factory_rgen);
            auto index_b = dist(_factory_rgen);

            parents.push_back(
                fitnesses[index_a] >= fitnesses[index_b]
                    ? population[index_a]
                    : population[index_b]);
        }

        return parents;
    }

    auto generate_offspring(Population &parents, CrossoverCb &crossover_cb) {
        auto offspring = Population{};

        for (auto i : range(parents.size())) {
            if (i % 2) {
                auto [child_a, child_b] = crossover_cb(
                    parents[i],
                    parents[i - 1]);

                offspring.push_back(child_a);
                offspring.push_back(child_b);

                continue;
            }

            if (i == parents.size() - 1) {
                auto [child_a, child_b] = crossover_cb(
                    parents[i],
                    parents[i - 1]);

                offspring.push_back(child_a);
            }
        }

        return offspring;
    }

public:
    auto generate_genetic_solution(
        int population_size,
        CrossoverCb &crossover_cb,
        MutationCb &mutation_cb,
        EndingConditionCb &ending_condition_cb) {

        auto population = this->generate_population(population_size);
        auto generation_count = 0;

        while (!ending_condition_cb(population, generation_count++)) {
            auto parents = this->select_parents(population);
            auto offspring = this->generate_offspring(parents, crossover_cb);

            auto mutated_offstpring = Population{};
            for (auto &solution : offspring) {
                mutated_offstpring.push_back(mutation_cb(solution));
            }

            population = std::move(mutated_offstpring);
        }

        return *std::max_element(
            population.begin(),
            population.end(),
            [&](auto a, auto b) {
                return this->calculate_fitness(a) < this->calculate_fitness(b);
            });
    }
};

using GarbageBagCountPerWeightMap = std::map<int, int>;

auto generate_bag_count_per_weight_map(GarbageBags bags) -> GarbageBagCountPerWeightMap {
    auto bag_count_per_weight_map = GarbageBagCountPerWeightMap{};

    for (auto &bag : bags) {
        auto weight = bag.get_weight();

        if (bag_count_per_weight_map.contains(weight)) {
            bag_count_per_weight_map[weight] += 1;
        } else {
            bag_count_per_weight_map[weight] = 1;
        }
    }

    return bag_count_per_weight_map;
}

auto BAG_COUNT_PER_WEIGHT_MAP = generate_bag_count_per_weight_map(GARBAGE_BAGS);

auto insert_available_bag_to_vec(
    GarbageBags &bags,
    GarbageBagCountPerWeightMap &bag_count_per_weight,
    GarbageBag bag,
    int target_index = -1) -> void {

    auto weight = bag.get_weight();

    if (
        bag_count_per_weight.contains(weight) &&
        bag_count_per_weight[weight] == BAG_COUNT_PER_WEIGHT_MAP[weight]) {
        return;
    }

    if (bag_count_per_weight.contains(weight)) {
        bag_count_per_weight[weight] += 1;
    } else {
        bag_count_per_weight[weight] = 1;
    }

    if (target_index == -1 || target_index >= bags.size()) {
        bags.push_back(std::move(bag));
    } else {
        bags.insert(bags.begin() + target_index, std::move(bag));
    }
}

auto find_bag_index_by_weight(GarbageBags &bags, int target_weight) {
    for (auto i : range(bags.size())) {
        if (bags[i].get_weight() == target_weight) {
            return i;
        }
    }

    return -1;
}

auto cross_parents_into_striped_children(
    Solution parent_a,
    Solution parent_b)
    -> SolutionPair {
    auto parent_bags_a = parent_a.get_garbage_bags();
    auto parent_bags_b = parent_b.get_garbage_bags();
    auto child_bags_a = GarbageBags{};
    auto child_bags_b = GarbageBags{};
    auto child_bag_count_per_weight_a = GarbageBagCountPerWeightMap{};
    auto child_bag_count_per_weight_b = GarbageBagCountPerWeightMap{};

    auto insert_to_child_bags_a = [&](GarbageBag bag, int target_index = -1) {
        insert_available_bag_to_vec(
            child_bags_a,
            child_bag_count_per_weight_a,
            std::move(bag),
            target_index);
    };

    auto insert_to_child_bags_b = [&](GarbageBag bag, int target_index = -1) {
        insert_available_bag_to_vec(
            child_bags_b,
            child_bag_count_per_weight_b,
            std::move(bag),
            target_index);
    };

    for (auto i : range(parent_bags_a.size())) {
        if (i % 2) {
            insert_to_child_bags_a(parent_bags_a[i]);
            insert_to_child_bags_b(parent_bags_b[i]);
        } else {
            insert_to_child_bags_a(parent_bags_b[i]);
            insert_to_child_bags_b(parent_bags_a[i]);
        }
    }

    for (auto [weight, count] : BAG_COUNT_PER_WEIGHT_MAP) {
        while (
            !child_bag_count_per_weight_a.contains(weight) ||
            child_bag_count_per_weight_a[weight] != count) {
            insert_to_child_bags_a(
                GarbageBag{weight},
                find_bag_index_by_weight(parent_bags_a, weight));
        }

        while (
            !child_bag_count_per_weight_b.contains(weight) ||
            child_bag_count_per_weight_b[weight] != count) {
            insert_to_child_bags_b(
                GarbageBag{weight},
                find_bag_index_by_weight(parent_bags_b, weight));
        }
    }

    auto child_a = Solution{BIN_WEIGHT_LIMIT, child_bags_a};
    auto child_b = Solution{BIN_WEIGHT_LIMIT, child_bags_b};

    return {std::move(child_a), std::move(child_b)};
}

auto swap_random_adjacent_bags(Solution solution) {
    return solution.generate_random_neighbor();
}

const auto GENERATION_COUNT_LIMIT = 10;

auto end_on_generation_count_limit(Population _, int generation_count) {
    return generation_count++ >= GENERATION_COUNT_LIMIT;
}

auto CROSSOVER_CB_MAP = std::map<int, CrossoverCb>{
    {1, cross_parents_into_striped_children},
};

auto MUTATION_CB_MAP = std::map<int, MutationCb>{
    {1, swap_random_adjacent_bags},
};

auto ENDING_CONDITION_CB_MAP = std::map<int, EndingConditionCb>{
    {1, end_on_generation_count_limit},
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    auto population_size = 100;
    auto crossover_cb_key = 1;
    auto mutation_cb_key = 1;
    auto ending_condition_cb_key = 1;

    if (argc >= 2) {
        if (std::string{argv[1]} == "help") {
            std::cout
                << "Args:"
                << std::endl
                << "1. Population size."
                << std::endl
                << "   Default: " << population_size
                << std::endl
                << "2. Crossover method."
                << std::endl
                << "   - 1 -> //todo @kw"
                << std::endl
                << "   - 2 -> //todo @kw"
                << std::endl
                << "   Default: " << crossover_cb_key
                << std::endl
                << "3. Mutation method."
                << std::endl
                << "   - 1 -> //todo @kw"
                << std::endl
                << "   - 2 -> //todo @kw"
                << std::endl
                << "   Default: " << mutation_cb_key
                << std::endl
                << "4. Ending condition method."
                << std::endl
                << "   - 1 -> //todo @kw"
                << std::endl
                << "   - 2 -> //todo @kw"
                << std::endl
                << "   Default: " << ending_condition_cb_key
                << std::endl;

            return 0;
        } else {
            population_size = std::stoi(argv[1]);

            if (argc >= 3) {
                auto new_crossover_cb_key = std::stoi(argv[2]);

                if (CROSSOVER_CB_MAP.contains(new_crossover_cb_key)) {
                    crossover_cb_key = new_crossover_cb_key;
                }

                if (argc >= 4) {
                    auto new_mutation_cb_key = std::stoi(argv[3]);

                    if (MUTATION_CB_MAP.contains(new_mutation_cb_key)) {
                        mutation_cb_key = new_mutation_cb_key;
                    }

                    if (argc >= 5) {
                        auto new_ending_condition_cb_key = std::stoi(argv[4]);

                        if (ENDING_CONDITION_CB_MAP.contains(new_ending_condition_cb_key)) {
                            ending_condition_cb_key = new_ending_condition_cb_key;
                        }
                    }
                }
            }
        }
    }

    std::cout
        << "Genetic solution:"
        << std::endl
        << solution_factory.generate_genetic_solution(
               population_size,
               CROSSOVER_CB_MAP[crossover_cb_key],
               MUTATION_CB_MAP[mutation_cb_key],
               ENDING_CONDITION_CB_MAP[ending_condition_cb_key])
        << std::endl;

    return 0;
}
