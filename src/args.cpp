#include "args.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include <filesystem>
#include <iostream>

using namespace crap;

Args::Args(int argc, char *argv[], const CommandInfos &infos)
    : commandInfos{infos} {
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
    else {
        if (method != "init") {
            locateRoot();
        }
    }

    method = args.at(0);

    args.erase(args.begin());

    if (!std::filesystem::is_directory(crap::potty)) {
        if (method != "init") {
            fmt::print(std::cerr, "potty is not initialized\n");
            std::exit(1);
        }
    }
}

void Args::locateRoot() {
    auto currentPath =
        std::filesystem::absolute(std::filesystem::current_path());

    for (; !currentPath.empty() && !std::filesystem::is_directory(potty);
         currentPath = currentPath.parent_path()) {

        if (std::filesystem::is_directory(currentPath / potty)) {
            std::filesystem::current_path(currentPath);
            return;
        }
    }

    fmt::print(std::cerr, "directory is not a valid crap potty\n");
    std::exit(1);
}

void Args::printHelp(int code) {
    auto commandDescription = std::string{};

    for (auto &info : commandInfos) {
        commandDescription +=
            fmt::format("  {:<15} {}\n", info.name, info.description);
    }

    auto helpMessage = fmt::format(R"(
Usage: crap [-C <dir>] <command> [options]

Commands:
{}

Options:
  -h, --help      Show this help message and exit
  -C <dir>        Work from the specified directory <dir>
)",
                                   commandDescription);
    std::cout << helpMessage << std::endl;

    std::exit(code);
}
