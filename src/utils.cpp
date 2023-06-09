#include "utils.h"
#include "GarbageBag.h"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

// using std::ranges::iota_view requires GCC v10
auto range(int len) -> std::vector<int> {
    auto v = std::vector<int>{};

    for (int i = 0; i < len; i++) {
        v.push_back(i);
    }

    return v;
}

auto print_help(const std::vector<ExpectedArg> &expected_args) -> void {
    std::cout << "Args:" << std::endl;

    for (auto i : range(expected_args.size())) {
        std::cout << i + 1 << ". " << expected_args[i].name << std::endl;

        if (expected_args[i].description.size()) {
            std::cout << "   " << expected_args[i].description << std::endl;
        }

        std::cout << "   Default: " << expected_args[i].default_value << std::endl;
    }
}

auto print_configuration(
    const std::vector<ExpectedArg> &expected_args,
    const std::vector<int> input_values)
    -> void {
    std::cout << "Configuration:" << std::endl;
    for (auto i : range(expected_args.size())) {
        std::cout << "  " << expected_args[i].name << ": " << input_values[i] << std::endl;
    }
}

auto collect_args(
    const std::vector<ExpectedArg> &expected_args,
    int argc,
    char *argv[])
    -> std::vector<int> {
    auto expected_args_size = expected_args.size();

    if (argc >= 2) {
        if (std::string{argv[1]} == "help") {
            print_help(expected_args);
            return std::vector<int>{};
        } else if (!expected_args_size) {
            std::cout << "No args allowed!" << std::endl;
            return std::vector<int>{};
        }
    }

    auto input_values = std::vector<int>{};

    for (auto i : range(expected_args_size)) {
        if (i + 1 < argc) {
            auto input_value = std::stoi(argv[i + 1]);

            if (!expected_args[i].allowed_values.size() ||
                expected_args[i].allowed_values.find(input_value) !=
                    expected_args[i].allowed_values.end()) {
                input_values.push_back(input_value);
                continue;
            }
        }

        input_values.push_back(expected_args[i].default_value);
    }

    print_configuration(expected_args, input_values);

    std::cout << std::endl;

    return input_values;
}

auto load_garbage_bags() -> std::vector<GarbageBag> {
    auto bags = std::vector<GarbageBag>{};

    auto data = std::fstream{"./data.txt", std::ios_base::in};

    int weight;
    while (data >> weight) {
        bags.push_back(GarbageBag{weight});
    }

    return bags;
}
