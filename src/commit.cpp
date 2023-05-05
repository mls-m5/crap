
#include "commit.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "hash.h"
#include "pottyutil.h"
#include "status.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <ranges>
#include <string>
#include <string_view>

namespace crap {

Commit::Commit(std::string hash) {
    loadFile(commitPath(hash));
}

Commit::Commit(std::string parent, std::vector<File> files, std::string message)
    : parent{std::move(parent)}
    , files{std::move(files)}
    , message{std::move(message)} {}

Commit Commit::loadDropped() {
    auto commit = Commit{};
    commit.loadFile(pottyFilePath());
    return commit;
}

std::string Commit::flush(std::filesystem::path path, bool temporary) {
    auto ss = std::ostringstream{};

    std::ranges::sort(files, [](auto &a, auto &b) { return a.path < b.path; });

    // Parent commit
    fmt::print(ss, "{}\n", butHash());
    fmt::print(ss, "{}\n", files.size());

    for (auto &file : files) {
        fmt::print(ss, "{} {}\n", file.hash, file.path.string());
    }

    if (message.empty()) {

        std::ofstream{commitMsgPath()} << "\n#dump message";

        std::system(
            fmt::format("vim \"{}\"", commitMsgPath().string()).c_str());
        auto ss = std::ostringstream{};
        ss << std::ifstream{commitMsgPath()}.rdbuf();
        message = ss.str();
    }

    message = cleanMessage(message);

    ss << message << "\n";

    auto str = ss.str();
    auto commitHash = hash(str);
    if (path.empty()) {
        path = commitPath(commitHash);
    }

    if (!temporary && std::filesystem::exists(path)) {
        fmt::print("exactly the commit {} already exist", path.string());
        return "";
    }

    std::ofstream{path} << str;

    if (!temporary) {
        std::ofstream{constants::butPath} << commitHash << "\n";
    }

    return commitHash;
}

void Commit::loadFile(std::filesystem::path path) {
    auto file = std::ifstream{path};
    std::getline(file, parent);

    std::string numFiles;
    std::getline(file, numFiles);

    if (numFiles.empty()) {
        return;
    }

    auto num = std::stol(numFiles);

    auto split = [](std::string_view str) {
        auto s = str.find(" ");

        return std::pair{str.substr(0, s), str.substr(s + 1)};
    };

    for (size_t i = 0; i < num; ++i) {
        std::string line;
        std::getline(file, line);

        auto [hash, name] = split(line);

        files.push_back({std::string{hash}, name, droppingsPath(hash)});
    }

    for (std::string line; std::getline(file, line);) {
        message += line + "\n";
    }

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

    while (!message.empty() && std::isspace(message.front())) {
        message.erase(0, 1);
    }
}

Commit stagedFiles(const Status &status) {
    auto filesView =
        status.staged | std::views::transform([](std::filesystem::path path) {
            return Commit::File{};
        });

    return Commit{butHash(),
                  std::vector<Commit::File>{filesView.begin(), filesView.end()},
                  ""};
}

} // namespace crap
