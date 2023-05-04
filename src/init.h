#pragma once

#include "args.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include <filesystem>
#include <iostream>

namespace crap {

inline int init(const Args &settings) {
    if (std::filesystem::is_directory(potty)) {
        fmt::print(std::cerr, "potty already exists... abort");
        return 1;
    }

    fmt::print("creating crap potty...");

    std::filesystem::create_directories(".crap/potty");

    return 0;
}

} // namespace crap
