#include "status.h"
#include "fmt/core.h"
#include <filesystem>

namespace crap {

int status(const Args &settings) {
    using namespace crap;

    fmt::print("Everything is just crappy");

    for (auto it = std::filesystem::recursive_directory_iterator{"."};
         it != std::filesystem::recursive_directory_iterator{};
         ++it) {

        if (it->path().filename() == ".crap") {
        }

        fmt::print("{}\n", it->path().string());
    }

    return 0;
}
} // namespace crap
