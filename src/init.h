#pragma once

#include "args.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include <filesystem>
#include <iostream>

namespace crap {

inline int init(const Args &settings) {
    if (std::filesystem::is_directory(pottyPath)) {
        fmt::print(std::cerr, "potty already exists... abort\n");
        return 1;
    }

    fmt::print("creating crap potty...\n");

    for (auto &dir : requiredDirectories) {
        std::filesystem::create_directories(dir);
    }

    return 0;
}

} // namespace crap
