#pragma once

#include "args.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace crap {

inline int init(const Args &settings) {
    if (std::filesystem::is_directory(constants::pottyPath)) {
        fmt::print(std::cerr, "potty already exists... abort\n");
        return 1;
    }

    for (auto &file : constants::requiredFiles) {
        if (std::filesystem::exists(file)) {
            fmt::print(
                std::cerr, "{} already exists... abort\n", file.string());
        }
    }

    fmt::print("creating crap potty...\n");

    for (auto &dir : constants::requiredDirectories) {
        std::filesystem::create_directories(dir);
    }

    for (auto &dir : constants::requiredFiles) {
        std::ofstream{dir} << "";
    }

    return 0;
}

} // namespace crap
