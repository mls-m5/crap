#include "add.h"
#include "commit.h"
#include "droppings.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "pottyutil.h"
#include "status.h"
#include <bits/ranges_algo.h>
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

    auto commit = Commit::loadDumped();

    for (auto &file : files) {
        auto ec = std::error_code{};

        if (ignore.shouldIgnore(file)) {
            fmt::print(
                std::cerr, "file '{}' is in ignore file\n", file.string());
            continue;
        }

        auto path = addDropping(file);

        if (auto f = commit.findFromPath(file)) {
            if (f->hash == path.filename()) {
                continue;
            }
            else {

                auto it =
                    std::remove_if(commit.files.begin(),
                                   commit.files.end(),
                                   [&file](auto &a) { return file == a.path; });
                commit.files.erase(it, commit.files.end());
            }
        }

        commit.files.push_back({path.filename(), file, path});

        fmt::print("dumped {}\n", file.string());
    }

    commit.message = "POTTY_TEMP_MESSAGE";
    commit.flush(pottyFilePath(), true);

    return 0;
}

} // namespace crap
