#include <vector>

#ifndef UTILS_H
#define UTILS_H

auto range(int len) {
    auto v = std::vector<int>{};

    for (int i = 0; i < len; i++) {
        v.push_back(i);
    }

    return v;
}

#endif // UTILS_H
