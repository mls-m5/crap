#include "flush.h"
#include "commit.h"
#include "fmt/core.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>

namespace crap {

namespace {

struct FlushArgs {
    std::string message;

    FlushArgs(const Args &args) {
        for (size_t i = 0; i < args.args.size(); ++i) {
            auto arg = args.args.at(i);
            if (arg == "--message" || arg == "-h") {
                message = args.args.at(++i);
            }
        }
    }
};

} // namespace

int flush(const Args &settings) {
    fmt::print("Flushing changes...\n");

    auto flushArgs = FlushArgs{settings};

    auto commit = Commit::loadDumped();
    commit.message = flushArgs.message;

    commit.flush();

    fmt::print("done...\n");

    return 0;
}

} // namespace crap
