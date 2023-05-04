#include "fmt/core.h"
#include "fmt/ostream.h"
#include "fmt/printf.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace {

struct Settings {
    std::vector<std::string> args;
    std::string method;

    Settings(int argc, char *argv[]) {
        args = std::vector<std::string>{argv + 1, argv + argc};

        if (args.empty()) {
            printHelp(1);
        }
        if (args.at(0) == "--help" || args.at(0) == "-h") {
            printHelp(0);
        }
        if (args.at(0) == "-C") {
            auto path = std::filesystem::path{args.at(1)};
            try {
                std::filesystem::current_path(path);
            }
            catch (std::runtime_error &e) {
                fmt::print(std::cerr,
                           "could not change working directory: \"{}\": {}",
                           path.string(),
                           e.what());
            }

            args.erase(args.begin(), args.begin() + 2);
        }

        method = args.at(0);
    }

    [[noreturn]] static void printHelp(int code) {
        std::string_view helpMessage = R"(
Usage: crap [-C <dir>] <command> [options]

Commands:
  commit    Commit changes
  flush     Flush changes

Options:
  -h, --help    Show this help message and exit
  -C <dir>      Work from the specified directory <dir>
)";
        std::cout << helpMessage << std::endl;

        std::exit(code);
    }
};

void commit(const Settings &settings) {
    fmt::print("Flushing changes...\n");
}

void flush(const Settings &settings) {
    fmt::print("Flushing changes...\n");
}

void status(const Settings &settings) {
    fmt::print("Everithing is just crappy");
}

using FT = decltype(commit) *;

} // namespace

int main(int argc, char *argv[]) {
    Settings settings(argc, argv);

    auto map = std::unordered_map<std::string, FT>{
        {"commit", commit},
        {"flush", flush},
        {"status", status},
    };

    if (auto f = map.find(settings.method); f != map.end()) {
        f->second(settings);
        return 0;
    }

    fmt::print(std::cerr, "invalid command: {}", settings.method);
    settings.printHelp(1);
}
