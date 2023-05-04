#pragma once

#include "args.h"
#include <filesystem>
#include <vector>

namespace crap {

struct Status {
    Status();

    struct File {
        using time_t = std::filesystem::file_time_type;
        File(std::filesystem::path path, time_t changedTime, time_t stagedTime)
            : path{path}
            , changedTime{changedTime}
            , stagedTime{stagedTime} {}

        std::filesystem::path path;
        std::filesystem::file_time_type changedTime = {};
        std::filesystem::file_time_type stagedTime = {};
    };

    std::vector<std::filesystem::path> added;
    std::vector<std::filesystem::path> deleted;
    std::vector<File> modified;
};

int status(const Args &settings);

} // namespace crap
