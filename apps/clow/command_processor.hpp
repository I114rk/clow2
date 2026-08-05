#pragma once

#include "command_parser.hpp"

namespace clow {

class CommandProcessor {
public:
    explicit CommandProcessor(const CommandParser& parser) noexcept;
    int execute();

private:
    int handleInstall(const std::vector<std::string>& arguments);
    int handleShell(const std::vector<std::string>& arguments);
    int handleExec(const std::vector<std::string>& arguments);
    int handleStop(const std::vector<std::string>& arguments);
    int handleRemove(const std::vector<std::string>& arguments);
    int handleList();
    int handleExport(const std::vector<std::string>& arguments);
    int handleImport(const std::vector<std::string>& arguments);
    int handleConfig();
    int handleUpdate(const std::vector<std::string>& arguments);

    const CommandParser& parser_;
};

} // namespace clow
