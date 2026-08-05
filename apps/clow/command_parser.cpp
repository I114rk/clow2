#include "command_parser.hpp"
#include "clow/logger.hpp"
#include <algorithm>

namespace clow {

CommandParser::CommandParser(int argc, char* argv[]) {
    args_.reserve(static_cast<size_t>(std::max(0, argc - 1)));
    for (int index = 1; index < argc; ++index) {
        args_.emplace_back(argv[index]);
    }
}

bool CommandParser::parse() noexcept {
    if (args_.empty()) {
        printUsage();
        return false;
    }
    command_ = args_.front();
    std::copy(std::next(args_.begin()), args_.end(), std::back_inserter(arguments_));
    return true;
}

std::string CommandParser::command() const noexcept {
    return command_;
}

std::vector<std::string> CommandParser::arguments() const noexcept {
    return arguments_;
}

void CommandParser::printUsage() noexcept {
    Logger::info("Usage: clow <command> [options]");
    Logger::info("Commands: install, shell, exec, stop, remove, list, export, import, config, update");
}

} // namespace clow
