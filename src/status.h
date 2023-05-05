#pragma once

#include "args.h"
#include "diff.h"
#include <filesystem>
#include <vector>

namespace crap {

// TODO: Handle wildcards
struct Ignore {
    std::vector<std::filesystem::path> paths;
    Ignore();

    bool shouldIgnore(const std::filesystem::path &path);
};

struct Status {
    Status();

    //    struct File {
    //        using time_t = std::filesystem::file_time_type;
    //        File(std::filesystem::path path,
    //             std::filesystem::path dropping,
    //             time_t changedTime,
    //             time_t stagedTime)
    //            : path{path}
    //            , dropping{dropping}
    //            , changedTime{changedTime}
    //            , stagedTime{stagedTime} {}

    //        std::filesystem::path path;
    //        std::filesystem::path dropping;
    //        std::filesystem::file_time_type changedTime = {};
    //        std::filesystem::file_time_type stagedTime = {};
    //    };

    //    std::vector<std::filesystem::path> added;
    //    std::vector<std::filesystem::path> deleted;
    //    std::vector<File> modified;
    //    std::vector<std::filesystem::path> staged;

    Diff undropped;
    Diff dropped;
};

int status(const Args &settings);

} // namespace crap
