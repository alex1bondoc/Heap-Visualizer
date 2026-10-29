#include "BlockInfo.h"
#include <charconv>

namespace {

// The text right after `key` in `object`, e.g. what follows "size": in {"size":64}.
std::optional<std::string_view> valueAfter(std::string_view object, std::string_view key) {
    size_t key_pos = object.find(key);
    if (key_pos == std::string_view::npos) {
        return std::nullopt;
    }
    return object.substr(key_pos + key.size());
}

}

// Input is what Heap::serialize() writes: [{"id":"0x...","status":"FREE","size":1024},...].
// The objects are flat, so each one is the text between a '{' and the next '}'.
// Keys can come in any order; "id" and "zone" are ignored.
std::optional<std::vector<BlockInfo>> parseBlocks(std::string_view json) {
    std::vector<BlockInfo> blocks;
    std::string_view remaining = json;
    while (true) {
        size_t open_pos = remaining.find('{');
        if (open_pos == std::string_view::npos) {
            break;
        }
        size_t close_pos = remaining.find('}', open_pos);
        if (close_pos == std::string_view::npos) {
            return std::nullopt;
        }
        std::string_view object = remaining.substr(open_pos + 1, close_pos - open_pos - 1);

        std::optional<std::string_view> size_str = valueAfter(object, "\"size\":");
        std::optional<std::string_view> status_str = valueAfter(object, "\"status\":\"");
        if (!size_str || !status_str) {
            return std::nullopt;
        }

        // The number is followed by ',' or the end of the object, so unlike
        // parseConfig the parse is not expected to reach the end of the text.
        int size{};
        auto result = std::from_chars(size_str->data(), size_str->data() + size_str->size(), size);
        if (result.ec != std::errc() || size <= 0) {
            return std::nullopt;
        }

        // The closing quote is part of the match, so "FREEZE" is not taken for "FREE".
        Status status{};
        if (status_str->starts_with("FREE\"")) {
            status = Status::FREE;
        } else if (status_str->starts_with("ALLOC\"")) {
            status = Status::ALLOC;
        } else {
            return std::nullopt;
        }

        blocks.push_back({size, status});
        remaining = remaining.substr(close_pos + 1);
    }
    return blocks;
}
