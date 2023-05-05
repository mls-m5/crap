#include "verify.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include <filesystem>
#include <iostream>

namespace crap {

bool verify() {
    if (!std::filesystem::is_directory(constants::pottyPath)) {
        fmt::print(std::cerr, "potty is not initialized\n");
        std::exit(1);
    }

    for (auto &dir : constants::requiredDirectories) {
        if (!std::filesystem::is_directory(dir)) {
            fmt::print("potty is corrupt: directory {} does not exist",
                       dir.string());
            return false;
        }
    }

    for (auto &dir : constants::requiredFiles) {
        if (!std::filesystem::is_regular_file(dir)) {
            fmt::print("potty is corrupt: file {} does not exist",
                       dir.string());
            return false;
        }
    }

    return true;
}

} // namespace crap
