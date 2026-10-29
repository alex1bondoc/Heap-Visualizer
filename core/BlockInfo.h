#pragma once 
#include "MemoryBlock.h"
#include <optional> 
#include <string_view>
#include <vector>

struct BlockInfo {
    int size;
    Status status;
};

std::optional<std::vector<BlockInfo>> parseBlocks(std::string_view json);