#include "status.h"
#include "constants.h"
#include "fmt/core.h"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>

namespace crap {

namespace {} // namespace

// TODO: Handle wildcards
std::vector<std::filesystem::path> loadIgnore() {
    auto ret = std::vector<std::filesystem::path>{};
    auto file = std::ifstream{".crapignore"};

    for (std::string line; std::getline(file, line);) {
        ret.push_back(line);
    }

    return ret;
}

int status(const Args &settings) {
    fmt::print("Potty is filthy\n");

    auto status = Status{};

    for (auto &a : status.added) {
        fmt::print("+ {}\n", a.string());
    }
    for (auto &d : status.deleted) {
        fmt::print("- {}\n", d.string());
    }
    for (auto &m : status.deleted) {
        fmt::print("modified {}\n", m.string());
    }
    return 0;
}

Status::Status() {
    auto ignore = loadIgnore();

    auto files = std::vector<std::filesystem::path>{};

    for (auto it = std::filesystem::recursive_directory_iterator{"."};
         it != std::filesystem::recursive_directory_iterator{};
         ++it) {

        if (it->path().filename() == ".crap") {
            it.disable_recursion_pending();
            continue;
        }

        auto path = std::filesystem::relative(it->path(), ".");

        if (auto it = std::find(ignore.begin(), ignore.end(), path);
            it != ignore.end()) {
            continue;
        }

        files.push_back(std::move(path));
    }

    std::ranges::sort(files);

    std::filesystem::create_directories(droppingsPath);

    auto staged = std::vector<std::filesystem::path>{};

    for (auto it = std::filesystem::recursive_directory_iterator{droppingsPath};
         it != std::filesystem::recursive_directory_iterator{};
         ++it) {

        auto path = std::filesystem::relative(it->path(), droppingsPath);

        staged.push_back(std::move(path));
    }

    std::ranges::sort(staged);

    {
        auto diff = std::vector<std::filesystem::path>{};
        std::ranges::set_difference(files, staged, std::back_inserter(diff));
        for (auto &d : diff) {
            added.push_back(d);
        }
    }
    {
        auto diff = std::vector<std::filesystem::path>{};
        std::ranges::set_difference(staged, files, std::back_inserter(diff));
        for (auto &d : diff) {
            deleted.push_back(d);
        }
    }

    {
        auto intersection = std::vector<std::filesystem::path>{};
        std::ranges::set_intersection(
            staged, files, std::back_inserter(intersection));
        for (auto &path : intersection) {
            auto changed1 = std::filesystem::last_write_time(path);
            auto changed2 = std::filesystem::last_write_time(pottyPath / path);
            modified.emplace_back(path, changed1, changed2);
        }
    }
}

} // namespace crap
