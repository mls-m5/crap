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

        bool operator<(const File &other) const {
            return path < other.path;
        }
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

    /// Load a commit that represent the dropped changes
    static Commit loadDumped();

    /// Load files on disk that is not dropped yet
    static Commit loadUndumped();

    /// Add the commit to the history of
    /// @return hash of the new commit
    /// @param path is only used to save temporary drop
    /// @param if the change is temporary and may overwrite old files
    std::string flush(std::filesystem::path path = {}, bool temporary = false);

private:
    Commit() = default;
    void loadFile(std::filesystem::path path);
};

// Commit stagedFiles(const Status &status);

// Commit unstagedFiles();

} // namespace crap
