#include "command_parser.hpp"
#include "command_processor.hpp"
#include "clow/logger.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        clow::Logger::progress("Starting CLOW CLI");
        clow::CommandParser parser(argc, argv);
        if (!parser.parse()) {
            return 1;
        }

        clow::CommandProcessor processor(parser);
        return processor.execute();
    } catch (const std::exception& ex) {
        clow::Logger::error(ex.what());
        return 1;
    }
}
