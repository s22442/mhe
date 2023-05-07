#include <iostream>
#include <map>
#include <set>
#include <vector>

#ifndef UTILS_H
#define UTILS_H

// using std::ranges::iota_view requires GCC v10
auto range(int len) -> std::vector<int>;

struct ExpectedArg {
    std::string name;
    std::string description;
    std::set<int> allowed_values;
    int default_value;
};

auto print_help(const std::vector<ExpectedArg> &expected_args) -> void;

auto print_configuration(
    const std::vector<ExpectedArg> &expected_args,
    const std::vector<int> input_values)
    -> void;

auto collect_args(
    const std::vector<ExpectedArg> &expected_args,
    int argc,
    char *argv[])
    -> std::vector<int>;

template <typename T>
inline auto map_keys_to_set(const std::map<int, T> &map) -> std::set<int> {
    auto set = std::set<int>{};

    for (auto &[key, _] : map) {
        set.insert(key);
    }

    return set;
};

#endif // UTILS_H
