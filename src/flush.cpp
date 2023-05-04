#include "flush.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "hash.h"
#include "status.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace crap {

namespace {

std::string butHash() {
    auto file = std::ifstream{butPath};
    std::string line;

    std::getline(file, line);

    return line;
}

} // namespace

int flush(const Args &settings) {
    fmt::print("Flushing changes...\n");

    auto status = Status{};

    auto ss = std::ostringstream{};

    // Parent commit
    fmt::print(ss, "{}\n", butHash());
    fmt::print(ss, "{}\n", status.staged.size());

    for (auto &file : status.staged) {
        auto hash = hashFile(file);
        auto dpath = droppingsPath / hash;

        fmt::print(ss, "{} {}\n", hash, file.string());
        if (!std::filesystem::exists(dpath)) {
            std::ofstream{dpath} << std::ifstream{file}.rdbuf();
        }
    }

    auto str = ss.str();

    auto commitHash = hash(str);

    std::ofstream{commitPath / commitHash} << str;

    std::ofstream{butPath} << commitHash;

    return 0;
}

} // namespace crap
