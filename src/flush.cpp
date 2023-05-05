#include "flush.h"
#include "commit.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "hash.h"
#include "pottyutil.h"
#include "status.h"
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

bool isPottyFilesChanged(const Status &status) {
    auto parentCommit = Commit{butHash()};
    for (auto &file : status.staged) {
        auto committedFile = parentCommit.findFromPath(file);
        if (!committedFile) {
            return true;
        }

        if (areFilesDifferent(pottyPath(file),
                              droppingsPath(committedFile->hash))) {
            return true;
        }
    }

    auto tf = [](const Commit::File &file) { return file.path; };

    auto transformedCommmitFiles =
        parentCommit.files | std::views::transform(tf);

    auto deleted = std::vector<std::filesystem::path>{};
    auto added = std::vector<std::filesystem::path>{};

    std::ranges::set_difference(
        status.staged, transformedCommmitFiles, std::back_inserter(deleted));

    if (!deleted.empty()) {
        return true;
    }

    std::ranges::set_difference(
        transformedCommmitFiles, status.staged, std::back_inserter(added));

    if (!added.empty()) {
        return true;
    }

    return false;
}

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

std::string cleanMessage(std::string message) {
    auto ss = std::istringstream{message};

    for (std::string line; std::getline(ss, line);) {
        if (line.empty()) {
        }
        else if (line.front() == '#') {
            continue;
        }
    }

    message = ss.str();

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

    while (!message.empty() && std::isspace(message.front())) {
        message.erase(0, 1);
    }

    return message;
}

} // namespace

int flush(const Args &settings) {
    fmt::print("Flushing changes...\n");

    auto flushArgs = FlushArgs{settings};

    auto status = Status{};

    if (!isPottyFilesChanged(status)) {
        fmt::print("no changes to put\n");
        return 1;
    }

    auto ss = std::ostringstream{};

    // Parent commit
    fmt::print(ss, "{}\n", butHash());
    fmt::print(ss, "{}\n", status.staged.size());

    for (auto &file : status.staged) {
        auto hash = hashFile(file);
        auto dpath = droppingsPath(hash);

        fmt::print(ss, "{} {}\n", hash, file.string());
        if (!std::filesystem::exists(dpath)) {
            std::ofstream{dpath} << std::ifstream{file}.rdbuf();
        }
    }

    auto str = ss.str();

    auto commitHash = hash(str);

    auto path = commitPath(commitHash);

    if (std::filesystem::exists(path)) {
        fmt::print("no changes to commit in current potty\n");
        return 1;
    }

    auto message = flushArgs.message;

    if (message.empty()) {

        std::ofstream{commitMsgPath()} << "\n#commit message";

        std::system(
            fmt::format("vim \"{}\"", commitMsgPath().string()).c_str());
        auto ss = std::ostringstream{};
        ss << std::ifstream{commitMsgPath()}.rdbuf();
        message = ss.str();
    }

    message = cleanMessage(message);

    if (message.empty()) {
        fmt::print(std::cerr, "no commit message, aborting...\n");
        return 1;
    }

    std::ofstream{path} << str << "\n" << message << "\n";

    std::ofstream{constants::butPath} << commitHash << "\n";

    return 0;
}

} // namespace crap
