#include "diff.h"
#include "constants.h"
#include "fmt/core.h"
#include "pottyutil.h"
#include "status.h"

namespace crap {

namespace {

void diffFiles(const std::string &file1,
               const std::string &file2,
               const std::string &label1,
               const std::string &label2) {
    std::string cmd = fmt::format(
        "diff --color=always -u --label \"{}\" --label \"{}\" {} {}",
        label1,
        label2,
        file1,
        file2);
    int result = std::system(cmd.c_str());

    if (result == -1) {
        std::cerr << "Error executing diff command." << std::endl;
    }
}
} // namespace

int diff(const Args &settings) {
    auto status = Status{};

    for (auto &a : status.added) {
        diffFiles("/dev/null", a, "/dev/null", ("b" / a));
    }

    for (auto &d : status.deleted) {
        diffFiles(d, "/dev/null", ("a" / d), "/dev/null");
    }

    for (auto &m : status.modified) {
        diffFiles(
            pottyPath(m.path), m.path, ("a" / m.path), ("b" / m.path));
    }

    return 0;
}

} // namespace crap
