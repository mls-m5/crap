#include "diff.h"
#include "fmt/core.h"
#include "pottyutil.h"
#include <algorithm>
#include <ranges>
#include <stdexcept>

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

struct DiffArgs {
    bool showDumped = false;

    DiffArgs(const Args &args) {
        for (size_t i = 0; i < args.args.size(); ++i) {
            auto arg = args.args.at(i);
            if (arg == "--dumped" || arg == "-d") {
                showDumped = true;
            }
        }
    }
};
} // namespace

int diff(const Args &settings) {
    auto args = DiffArgs{settings};
    auto diff = args.showDumped
                    ? Diff{Commit{butHash()}, Commit::loadDumped()}
                    : Diff{Commit::loadDumped(), Commit::loadUndumped()};

    for (auto &a : diff.added) {
        diffFiles("/dev/null", a, "/dev/null", ("b" / a));
    }

    for (auto &d : diff.deleted) {
        diffFiles(d, "/dev/null", ("a" / d), "/dev/null");
    }

    for (auto &m : diff.modified) {
        diffFiles(m.a, m.b, ("a" / m.path), ("b" / m.path));
    }

    return 0;
}

Diff::Diff(const Commit &a, const Commit &b) {
    {
        auto deleted = std::vector<Commit::File>{};
        std::set_difference(a.files.begin(),
                            a.files.end(),
                            b.files.begin(),
                            b.files.end(),
                            std::back_inserter(deleted));

        for (auto &d : deleted) {
            this->deleted.push_back(d.path);
        }
    }
    {
        auto added = std::vector<Commit::File>{};
        std::set_difference(b.files.begin(),
                            b.files.end(),
                            a.files.begin(),
                            a.files.end(),
                            std::back_inserter(added));

        for (auto &d : added) {
            this->added.push_back(d.path);
        }
    }

    {

        // Chat gpt magic
        for (auto it1 = b.files.begin(), it2 = a.files.begin();
             it1 != b.files.end() && it2 != a.files.end();) {
            if (*it1 < *it2) {
                ++it1;
            }
            else if (*it2 < *it1) {
                ++it2;
            }
            else {

                if (it1->hash != it2->hash) {
                    modified.push_back({it1->path, it1->path, it2->path});
                }
                ++it1;
                ++it2;
            }
        }
    }
}

} // namespace crap
