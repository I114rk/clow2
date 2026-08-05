#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace clow {

class JsonValue;
using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

/// Represents a JSON value of primitive, array, or object type.
class JsonValue {
public:
    using ValueType = std::variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject>;

    JsonValue() noexcept;
    JsonValue(std::nullptr_t) noexcept;
    JsonValue(bool boolean) noexcept;
    JsonValue(int integer) noexcept;
    JsonValue(double number) noexcept;
    JsonValue(std::string value);
    JsonValue(JsonArray array);
    JsonValue(JsonObject object);

    bool isNull() const noexcept;
    bool isBool() const noexcept;
    bool isNumber() const noexcept;
    bool isString() const noexcept;
    bool isArray() const noexcept;
    bool isObject() const noexcept;

    bool asBool() const;
    double asNumber() const;
    const std::string& asString() const;
    const JsonArray& asArray() const;
    const JsonObject& asObject() const;

    std::string toString() const;
    static JsonValue parse(std::string_view payload);

private:
    ValueType value_;
};

} // namespace clow
