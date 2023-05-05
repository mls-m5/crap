#include "status.h"
#include "commit.h"
#include "diff.h"
#include "fmt/core.h"
#include "pottyutil.h"
#include <algorithm>
#include <array>
#include <bits/ranges_algo.h>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>

namespace crap {

const std::string_view red = "\033[31m";
const std::string_view green = "\033[32m";
const std::string_view reset = "\033[0m";

int status(const Args &settings) {
    auto status = Status{};
    if (status.dropped.isChanged()) {
        fmt::print("Dumped files ready to flush:\n");

        std::cout << green;
        for (auto &a : status.dropped.added) {
            fmt::print("+ {}\n", a.string());
        }
        for (auto &d : status.dropped.deleted) {
            fmt::print("- {}\n", d.string());
        }
        for (auto &m : status.dropped.modified) {
            fmt::print("m {}\n", m.path.string());
        }
        fmt::print("{}---\n", reset);
    }

    if (status.dropped.isChanged() || status.undropped.isChanged()) {
        fmt::print("Potty is filthy 🤢\n");
    }
    else {
        fmt::print("Potty is fresh 😇\n");
    }

    std::cout << red;
    for (auto &a : status.undropped.added) {
        fmt::print("+ {}\n", a.string());
    }
    for (auto &d : status.undropped.deleted) {
        fmt::print("- {}\n", d.string());
    }
    for (auto &m : status.undropped.modified) {
        fmt::print("m {}\n", m.path.string());
    }

    std::cout << reset;
    return 0;
}

Status::Status() {
    auto undropped = Commit::loadUndumped();
    auto dropped = Commit::loadDumped();
    auto but = Commit{butHash()};

    this->dropped = Diff{but, dropped};
    this->undropped = Diff{dropped, undropped};
}

Ignore::Ignore() {
    auto file = std::ifstream{".crapignore"};

    for (std::string line; std::getline(file, line);) {
        paths.push_back(line);
    }
}

bool Ignore::shouldIgnore(const std::filesystem::path &path) {
    if (auto it = std::find(paths.begin(), paths.end(), path);
        it != paths.end()) {
        return true;
    }
    return false;
}

} // namespace crap
