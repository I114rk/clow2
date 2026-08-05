#include "clow/configuration.hpp"
#include "clow/json.hpp"
#include <cassert>
#include <iostream>

int main() {
    clow::Configuration config = clow::Configuration::defaultConfiguration();
    config.setHostname("clow-host");
    config.setRamMb(8192);
    auto json = config.toJson();
    auto restored = clow::Configuration::fromJson(json);
    assert(restored.hostname() == "clow-host");
    assert(restored.ramMb() == 8192);
    std::cout << "test_configuration passed\n";
    return 0;
}
