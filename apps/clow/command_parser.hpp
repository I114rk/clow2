#pragma once

#include <string>
#include <vector>

namespace clow {

class CommandParser {
public:
    CommandParser(int argc, char* argv[]);
    bool parse() noexcept;

    std::string command() const noexcept;
    std::vector<std::string> arguments() const noexcept;

    static void printUsage() noexcept;

private:
    std::vector<std::string> args_;
    std::string command_;
    std::vector<std::string> arguments_;
};

} // namespace clow
