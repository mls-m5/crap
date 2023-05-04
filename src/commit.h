#pragma once

#include <filesystem>
#include <vector>

namespace crap {
/// Load information about a single flush into memory
struct Commit {
    Commit(std::string hash);

    struct File {
        std::string hash;
        std::filesystem::path path;
    };

    std::vector<File> files;
    std::string parent;
    std::string message;
};

} // namespace crap
