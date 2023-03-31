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

using CrossoverCb = std::function<std::vector<Solution>(std::vector<Solution>)>;
using MutationCb = std::function<Solution(Solution)>;
using EndingConditionCb = std::function<bool(std::vector<Solution>)>;

auto _factory_rd = std::random_device{};
auto _factory_rgen = std::mt19937{_factory_rd()};

class SolutionFactory {
private:
    auto generate_random_solution() {
        auto bags = GARBAGE_BAGS;
        std::shuffle(bags.begin(), bags.end(), _factory_rgen);
        return Solution{BIN_WEIGHT_LIMIT, bags};
    }

    auto generate_population(int population_size) {
        auto population = std::vector<Solution>{};

        for (auto _ : range(population_size)) {
            population.push_back(this->generate_random_solution());
        }

        return population;
    }

    auto calculate_fitness(Solution solution) {
        return 1.0 / (1 + solution.get_filled_bin_count());
    }

    auto select_parents(std::vector<Solution> population) {
        std::vector<int> fitnesses;

        for (int i = 0; i < population.size(); i++) {
            fitnesses.push_back(this->calculate_fitness(population[i]));
        }

        return population;
    }

public:
    auto generate_genetic_solution(
        int population_size,
        CrossoverCb crossover_cb,
        MutationCb mutation_cb,
        EndingConditionCb ending_condition_cb) {

        auto population = this->generate_population(population_size);

        while (!ending_condition_cb(population)) {
            auto parents = this->select_parents(population);
            auto offspring = crossover_cb(parents);

            auto mutated_offstpring = std::vector<Solution>{};
            std::transform(
                offspring.begin(),
                offspring.end(),
                mutated_offstpring.begin(),
                [&](auto solution) { return mutation_cb(solution); });

            population = offspring;
        }

        return *std::max_element(
            population.begin(),
            population.end(),
            [&](auto a, auto b) {
                return this->calculate_fitness(a) > this->calculate_fitness(b);
            });
    }
};

// todo @kw
const auto TMP_CROSSOVER_CB = CrossoverCb{
    [](auto parents) {
        return parents;
    }};

// todo @kw
const auto TMP_MUTATION_CB = MutationCb{
    [](auto solution) {
        return solution;
    }};

// todo @kw
const auto TMP_ENDING_CONDITION_CB = EndingConditionCb{
    [](auto population) {
        return true;
    }};

auto CROSSOVER_CB_MAP = std::map<int, CrossoverCb>{
    {1, TMP_CROSSOVER_CB},
};

auto MUTATION_CB_MAP = std::map<int, MutationCb>{
    {1, TMP_MUTATION_CB},
};

auto ENDING_CONDITION_CB_MAP = std::map<int, EndingConditionCb>{
    {1, TMP_ENDING_CONDITION_CB},
};

int main(int argc, char *argv[]) {
    auto solution_factory = SolutionFactory{};

    auto population_size = 10;
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
