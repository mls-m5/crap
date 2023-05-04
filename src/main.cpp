#include "args.h"
#include "init.h"
#include "status.h"
#include <string_view>
#include <unordered_map>

using namespace crap;

namespace {

int commit(const Args &settings) {
    fmt::print("Flushing changes...\n");
    return 0;
}

int flush(const Args &settings) {
    fmt::print("Flushing changes...\n");
    return 0;
}

int shame(const Args &settings) {
    fmt::print("Shame on you\n");
    return 0;
}

} // namespace

int main(int argc, char *argv[]) {
    const auto infos = std::vector<Args::CommandInfo>{
        {"init", init, "Create a fresh crap potty"},
        {"commit", commit, "Save changes in potty"},
        {"add", commit, "Sage changes for commit"},
        {"flush", flush, "Flush changes"},
        {"status", status, "Show the state of the current potty"},
        {"shame", shame, "Find out who is guilty of a specific change"},
    };

    Args settings(argc, argv, infos);

    for (auto &info : infos) {
        if (info.name == settings.method) {
            return info.f(settings);
        }
    }

    fmt::print(std::cerr, "invalid command: {}", settings.method);
    settings.printHelp(1);
}
