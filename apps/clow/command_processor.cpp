#include "command_processor.hpp"
#include "clow/installer.hpp"
#include "clow/logger.hpp"
#include <stdexcept>

namespace clow {

CommandProcessor::CommandProcessor(const CommandParser& parser) noexcept : parser_(parser) {}

int CommandProcessor::execute() {
    const auto command = parser_.command();
    const auto arguments = parser_.arguments();
    Installer installer;

    if (command == "install") {
        if (arguments.empty()) {
            Logger::error("install requires a distribution name");
            return 1;
        }
        std::string name = arguments[0];
        for (size_t i = 1; i < arguments.size(); ++i) {
            name += ' ';
            name += arguments[i];
        }
        return installer.installDistribution(name);
    }
    if (command == "shell") {
        if (arguments.size() != 1) {
            Logger::error("shell requires exactly one distribution name");
            return 1;
        }
        return installer.launchShell(arguments[0]);
    }
    if (command == "exec") {
        if (arguments.size() < 2) {
            Logger::error("exec requires a distribution name and a command");
            return 1;
        }
        std::string commandString;
        for (size_t i = 1; i < arguments.size(); ++i) {
            if (i > 1) {
                commandString += ' ';
            }
            commandString += arguments[i];
        }
        return installer.executeCommand(arguments[0], commandString);
    }
    if (command == "stop") {
        if (arguments.size() != 1) {
            Logger::error("stop requires exactly one distribution name");
            return 1;
        }
        return installer.stopDistribution(arguments[0]);
    }
    if (command == "remove") {
        if (arguments.size() != 1) {
            Logger::error("remove requires exactly one distribution name");
            return 1;
        }
        return installer.removeDistribution(arguments[0]);
    }
    if (command == "list") {
        return installer.listDistributions();
    }
    if (command == "export") {
        if (arguments.size() != 2) {
            Logger::error("export requires a distribution name and archive path");
            return 1;
        }
        return installer.exportDistribution(arguments[0], arguments[1]);
    }
    if (command == "import") {
        if (arguments.size() != 1) {
            Logger::error("import requires a path to an archive");
            return 1;
        }
        return installer.importDistribution(arguments[0]);
    }
    if (command == "config") {
        return installer.showConfiguration();
    }
    if (command == "update") {
        if (arguments.size() != 1) {
            Logger::error("update requires exactly one distribution name");
            return 1;
        }
        return installer.updateDistribution(arguments[0]);
    }

    Logger::error("Unknown command: " + command);
    return 1;
}

} // namespace clow
