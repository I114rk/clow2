#include "clow/json.hpp"
#include <charconv>
#include <cctype>
#include <stdexcept>

namespace clow {

namespace {

void skipWhitespace(std::string_view& text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
        text.remove_prefix(1);
    }
}

JsonValue parseValue(std::string_view& text);

JsonValue parseString(std::string_view& text) {
    if (text.empty() || text.front() != '"') {
        throw std::runtime_error("Invalid JSON string start");
    }
    text.remove_prefix(1);
    std::string result;
    while (!text.empty() && text.front() != '"') {
        if (text.front() == '\\') {
            text.remove_prefix(1);
            if (text.empty()) {
                throw std::runtime_error("Invalid JSON escape sequence");
            }
            char escaped = text.front();
            text.remove_prefix(1);
            switch (escaped) {
                case '"': result.push_back('"'); break;
                case '\\': result.push_back('\\'); break;
                case '/': result.push_back('/'); break;
                case 'b': result.push_back('\b'); break;
                case 'f': result.push_back('\f'); break;
                case 'n': result.push_back('\n'); break;
                case 'r': result.push_back('\r'); break;
                case 't': result.push_back('\t'); break;
                default:
                    throw std::runtime_error("Unsupported JSON escape sequence");
            }
            continue;
        }
        result.push_back(text.front());
        text.remove_prefix(1);
    }
    if (text.empty() || text.front() != '"') {
        throw std::runtime_error("Unterminated JSON string");
    }
    text.remove_prefix(1);
    return JsonValue(std::move(result));
}

JsonValue parseNumber(std::string_view& text) {
    const char* begin = text.data();
    const char* end = text.data() + text.size();
    double value = 0.0;
    auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc()) {
        throw std::runtime_error("Invalid JSON number");
    }
    std::size_t consumed = static_cast<std::size_t>(result.ptr - begin);
    text.remove_prefix(consumed);
    return JsonValue(value);
}

JsonValue parseArray(std::string_view& text) {
    text.remove_prefix(1);
    JsonArray array;
    skipWhitespace(text);
    if (!text.empty() && text.front() == ']') {
        text.remove_prefix(1);
        return JsonValue(std::move(array));
    }
    while (true) {
        array.push_back(parseValue(text));
        skipWhitespace(text);
        if (text.empty()) {
            throw std::runtime_error("Unterminated JSON array");
        }
        if (text.front() == ']') {
            text.remove_prefix(1);
            break;
        }
        if (text.front() != ',') {
            throw std::runtime_error("Expected comma in JSON array");
        }
        text.remove_prefix(1);
    }
    return JsonValue(std::move(array));
}

JsonValue parseObject(std::string_view& text) {
    text.remove_prefix(1);
    JsonObject object;
    skipWhitespace(text);
    if (!text.empty() && text.front() == '}') {
        text.remove_prefix(1);
        return JsonValue(std::move(object));
    }
    while (true) {
        skipWhitespace(text);
        auto key = parseString(text).asString();
        skipWhitespace(text);
        if (text.empty() || text.front() != ':') {
            throw std::runtime_error("Expected colon in JSON object");
        }
        text.remove_prefix(1);
        skipWhitespace(text);
        object.emplace(key, parseValue(text));
        skipWhitespace(text);
        if (text.empty()) {
            throw std::runtime_error("Unterminated JSON object");
        }
        if (text.front() == '}') {
            text.remove_prefix(1);
            break;
        }
        if (text.front() != ',') {
            throw std::runtime_error("Expected comma in JSON object");
        }
        text.remove_prefix(1);
    }
    return JsonValue(std::move(object));
}

JsonValue parseValue(std::string_view& text) {
    skipWhitespace(text);
    if (text.empty()) {
        throw std::runtime_error("Unexpected end of JSON payload");
    }
    switch (text.front()) {
        case 'n':
            if (text.rfind("null", 0) == 0) {
                text.remove_prefix(4);
                return JsonValue(nullptr);
            }
            break;
        case 't':
            if (text.rfind("true", 0) == 0) {
                text.remove_prefix(4);
                return JsonValue(true);
            }
            break;
        case 'f':
            if (text.rfind("false", 0) == 0) {
                text.remove_prefix(5);
                return JsonValue(false);
            }
            break;
        case '"':
            return parseString(text);
        case '[':
            return parseArray(text);
        case '{':
            return parseObject(text);
        default:
            return parseNumber(text);
    }
    throw std::runtime_error("Invalid JSON token");
}

JsonValue::JsonValue() noexcept : value_(nullptr) {}
JsonValue::JsonValue(std::nullptr_t) noexcept : value_(nullptr) {}
JsonValue::JsonValue(bool boolean) noexcept : value_(boolean) {}
JsonValue::JsonValue(int integer) noexcept : value_(static_cast<double>(integer)) {}
JsonValue::JsonValue(double number) noexcept : value_(number) {}
JsonValue::JsonValue(std::string value) : value_(std::move(value)) {}
JsonValue::JsonValue(JsonArray array) : value_(std::move(array)) {}
JsonValue::JsonValue(JsonObject object) : value_(std::move(object)) {}

bool JsonValue::isNull() const noexcept { return std::holds_alternative<std::nullptr_t>(value_); }
bool JsonValue::isBool() const noexcept { return std::holds_alternative<bool>(value_); }
bool JsonValue::isNumber() const noexcept { return std::holds_alternative<double>(value_); }
bool JsonValue::isString() const noexcept { return std::holds_alternative<std::string>(value_); }
bool JsonValue::isArray() const noexcept { return std::holds_alternative<JsonArray>(value_); }
bool JsonValue::isObject() const noexcept { return std::holds_alternative<JsonObject>(value_); }

bool JsonValue::asBool() const { return std::get<bool>(value_); }
double JsonValue::asNumber() const { return std::get<double>(value_); }
const std::string& JsonValue::asString() const { return std::get<std::string>(value_); }
const JsonArray& JsonValue::asArray() const { return std::get<JsonArray>(value_); }
const JsonObject& JsonValue::asObject() const { return std::get<JsonObject>(value_); }

std::string JsonValue::toString() const {
    if (isNull()) {
        return "null";
    }
    if (isBool()) {
        return asBool() ? "true" : "false";
    }
    if (isNumber()) {
        auto value = asNumber();
        std::string buffer(32, '\0');
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
        if (ec == std::errc()) {
            return std::string(buffer.data(), ptr);
        }
        return "0";
    }
    if (isString()) {
        return '"' + asString() + '"';
    }
    if (isArray()) {
        std::string result = "[";
        const auto& items = asArray();
        for (std::size_t index = 0; index < items.size(); ++index) {
            result += items[index].toString();
            if (index + 1 < items.size()) {
                result += ",";
            }
        }
        result += "]";
        return result;
    }
    if (isObject()) {
        std::string result = "{";
        const auto& members = asObject();
        std::size_t count = 0;
        for (const auto& [key, value] : members) {
            result += '"' + key + '"';
            result += ':';
            result += value.toString();
            if (++count < members.size()) {
                result += ',';
            }
        }
        result += "}";
        return result;
    }
    return "null";
}

JsonValue JsonValue::parse(std::string_view payload) {
    skipWhitespace(payload);
    auto result = parseValue(payload);
    skipWhitespace(payload);
    if (!payload.empty()) {
        throw std::runtime_error("Extra data after JSON payload");
    }
    return result;
}

} // namespace clow
