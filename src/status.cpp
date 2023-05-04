#include "status.h"
#include "constants.h"
#include "fmt/core.h"
#include "hash.h"
#include <algorithm>
#include <array>
#include <bits/ranges_algo.h>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>

namespace {

bool are_files_different(const std::filesystem::path &file_path1,
                         const std::filesystem::path &file_path2) {
    // Compare file sizes
    auto file_size1 = std::filesystem::file_size(file_path1);
    auto file_size2 = std::filesystem::file_size(file_path2);

    if (file_size1 != file_size2) {
        return true;
    }

    // Compare file contents
    std::ifstream file1(file_path1, std::ios::binary);
    std::ifstream file2(file_path2, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        throw std::runtime_error(
            "Unable to open one or both files for comparison.");
    }

    constexpr size_t buffer_size = 4096;
    std::array<char, buffer_size> buffer1 = {};
    std::array<char, buffer_size> buffer2 = {};

    while (file1.read(buffer1.data(), buffer_size) &&
           file2.read(buffer2.data(), buffer_size)) {
        if (buffer1 != buffer2) {
            return true;
        }

        file1 = {};
        file2 = {};
    }

    return false;
}

} // namespace

namespace crap {

int status(const Args &settings) {
    auto status = Status{};
    if (!status.added.empty() || !status.deleted.empty() ||
        !status.modified.empty()) {
        fmt::print("Potty is filthy\n");
    }

    for (auto &a : status.added) {
        fmt::print("+ {}\n", a.string());
    }
    for (auto &d : status.deleted) {
        fmt::print("- {}\n", d.string());
    }
    for (auto &m : status.modified) {
        fmt::print("m {} file hash\n", m.path.string());
    }
    return 0;
}

Status::Status() {
    auto ignore = Ignore();

    auto files = std::vector<std::filesystem::path>{};

    for (auto it = std::filesystem::recursive_directory_iterator{"."};
         it != std::filesystem::recursive_directory_iterator{};
         ++it) {

        if (it->path().filename() == ".crap") {
            it.disable_recursion_pending();
            continue;
        }

        auto path = std::filesystem::relative(it->path(), ".");

        if (ignore.shouldIgnore(path)) {
            continue;
        }

        files.push_back(std::move(path));
    }

    std::ranges::sort(files);

    staged = std::vector<std::filesystem::path>{};

    for (auto it = std::filesystem::recursive_directory_iterator{pottyPath};
         it != std::filesystem::recursive_directory_iterator{};
         ++it) {

        auto path = std::filesystem::relative(it->path(), pottyPath);

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
            auto stagedPath = pottyPath / path;
            auto changed1 = std::filesystem::last_write_time(path);
            auto changed2 = std::filesystem::last_write_time(stagedPath);

            if (changed1 != changed2) {
                if (are_files_different(path, stagedPath)) {
                    modified.emplace_back(path, changed1, changed2);
                }
                else {
                    std::filesystem::copy(
                        path,
                        stagedPath,
                        std::filesystem::copy_options::overwrite_existing);

                    std::filesystem::last_write_time(stagedPath, changed1);
                }
            }
        }
    }
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
