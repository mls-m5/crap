#pragma once

#include "args.h"
#include "commit.h"
#include <vector>

namespace crap {

int diff(const Args &settings);

struct Diff {
    struct File {
        std::filesystem::path path;
        std::filesystem::path a;
        std::filesystem::path b;
    };

    std::vector<std::filesystem::path> added;
    std::vector<std::filesystem::path> deleted;
    std::vector<File> modified;

    bool isChanged() {
        return !(added.empty() && deleted.empty() && modified.empty());
    }

    Diff(const Commit &a, const Commit &b);

    Diff() = default;
};

} // namespace crap
