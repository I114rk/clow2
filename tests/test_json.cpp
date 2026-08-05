#include "clow/json.hpp"
#include <cassert>
#include <iostream>

int main() {
    const auto json = clow::JsonValue::parse("{\"name\":\"Arch Linux\",\"default_ram\":4096}");
    assert(json.isObject());
    const auto& object = json.asObject();
    assert(object.at("name").asString() == "Arch Linux");
    assert(static_cast<int>(object.at("default_ram").asNumber()) == 4096);
    std::cout << "test_json passed\n";
    return 0;
}
