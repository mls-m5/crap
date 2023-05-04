#include "add.h"
#include "args.h"
#include "init.h"
#include "status.h"
#include <string_view>

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
        {"dump", add, "Dump into the potty"},
        {"flush", flush, "Flush changes into tank"},
        {"status", status, "Show the state of the current potty"},
        {"shame", shame, "Find out who is guilty of a specific change"},
    };

    const auto corrections = std::vector<std::pair<std::string, std::string>>{
        {"add", "dump"},
        {"commit", "flush"},
        {"blame", "shame"},
    };

    Args settings(argc, argv, infos);

    for (auto &info : infos) {
        if (info.name == settings.method) {
            return info.f(settings);
        }
    }

    for (auto &correction : corrections) {
        if (correction.first == settings.method) {

            fmt::print(std::cerr,
                       "Command '{}' does not exist, did you mean '{}'? See "
                       "'crap --help'\n",
                       settings.method,
                       correction.second);
            return 1;
        }
    }

    fmt::print(std::cerr, "Command '{}' does not exist\n", settings.method);
    settings.printHelp(1);
}
