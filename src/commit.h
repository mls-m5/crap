#pragma once

#include <filesystem>
#include <vector>

namespace crap {

struct Status;

/// Load information about a single flush into memory
struct Commit {
    struct File {
        std::string hash;
        std::filesystem::path path;
        std::filesystem::path realPath;
    };

    Commit(std::string hash);
    Commit(std::string parent, std::vector<File> files, std::string message);

    File *findFromPath(const std::filesystem::path &path) {
        for (auto &file : files) {
            if (file.path == path) {
                return &file;
            }
        }

        return nullptr;
    }

    std::vector<File> files;
    std::string parent;
    std::string message;
};

Commit stagedFiles(const Status &status);

Commit unstagedFiles();

} // namespace crap
