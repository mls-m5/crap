#include "log.h"
#include "commit.h"
#include "fmt/core.h"
#include "pottyutil.h"
#include <fstream>
#include <string>
#include <string_view>

namespace crap {

int log(const Args &settings) {

    auto nextHash = [](std::string_view hash) {
        auto path = strToCommitPath(hash);

        std::string newHash;
        auto file = std::ifstream{path};

        std::getline(file, newHash);

        return newHash;
    };

    for (auto hash = butHash(); !hash.empty();) {

        auto commit = Commit{hash};

        fmt::print("{} {}\n", hash.substr(0, 8), commit.message);

        hash = commit.parent;
    }

    return 0;
}

} // namespace crap
