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
        fmt::print(std::cerr, "potty already exists... abort");
        return 1;
    }

    fmt::print("creating crap potty...");

    std::filesystem::create_directories(pottyPath);
    std::filesystem::create_directories(droppingsPath);
    std::filesystem::create_directories(commitPath);

    return 0;
}

} // namespace crap
