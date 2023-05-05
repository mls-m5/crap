#include "flush.h"
#include "constants.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "hash.h"
#include "pottyutil.h"
#include "status.h"
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>

namespace crap {

namespace {

bool isPottyFilesChanged(const Status &status) {}

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
        auto dpath = droppingsPath(hash);

        fmt::print(ss, "{} {}\n", hash, file.string());
        if (!std::filesystem::exists(dpath)) {
            std::ofstream{dpath} << std::ifstream{file}.rdbuf();
        }
    }

    auto str = ss.str();

    auto commitHash = hash(str);

    auto path = strToCommitPath(commitHash);

    if (std::filesystem::exists(path)) {
        fmt::print("no changes to commit in current potty");
        return 1;
    }

    std::ofstream{commitMsgPath()} << "\n#commit message";

    std::system(fmt::format("vim \"{}\"", commitMsgPath().string()).c_str());

    std::ofstream{path} << str << "\n"
                        << std::ifstream{commitMsgPath()}.rdbuf() << "\n";

    std::ofstream{constants::butPath} << commitHash << "\n";

    return 0;
}

} // namespace crap
