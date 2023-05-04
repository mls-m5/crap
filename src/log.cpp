#include "log.h"
#include "constants.h"
#include "fmt/core.h"
#include "pottyutil.h"
#include <fstream>
#include <string>
#include <string_view>

namespace crap {

int log(const Args &settings) {

    auto nextHash = [](std::string_view hash) {
        auto path = commitPath / hash;

        std::string newHash;
        auto file = std::ifstream{path};

        std::getline(file, newHash);

        return newHash;
    };

    for (auto hash = butHash(); !hash.empty(); hash = nextHash(hash)) {
        fmt::print("{}\n", hash);
    }

    return 0;
}

} // namespace crap
