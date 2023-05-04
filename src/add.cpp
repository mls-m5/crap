#include "add.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "status.h"
#include <filesystem>
#include <iostream>
#include <system_error>
#include <vector>

namespace crap {

int add(const crap::Args &args) {
    auto files =
        std::vector<std::filesystem::path>{args.args.begin(), args.args.end()};

    if (files.empty()) {
        fmt::print(std::cerr, "No files specified\n");
        return 1;
    }

    for (auto &file : files) {
        if (!std::filesystem::exists(file)) {
            fmt::print(std::cerr, "file {} does not exist\n", file.string());
            return 1;
        }
    }

    auto ignore = Ignore{};

    for (auto &file : files) {
        auto ec = std::error_code{};

        if (ignore.shouldIgnore(file)) {
            fmt::print(
                std::cerr, "file '{}' is in ignore file\n", file.string());
            continue;
        }

        std::filesystem::copy(file,
                              pottyPath / file,
                              std::filesystem::copy_options::overwrite_existing,
                              ec);

        if (ec) {
            fmt::print(std::cerr, "failed to add '{}'\n", file.string());
            return 1;
        }

        fmt::print("added {}\n", file.string());
    }

    return 0;
}

} // namespace crap
