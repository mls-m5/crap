#pragma once

#include "args.h"
#include "diff.h"
#include <filesystem>
#include <vector>

namespace crap {

// TODO: Handle wildcards
struct Ignore {
    std::vector<std::filesystem::path> paths;
    Ignore();

    bool shouldIgnore(const std::filesystem::path &path);
};

struct Status {
    Status();

    Diff undropped;
    Diff dropped;
};

int status(const Args &settings);

} // namespace crap
