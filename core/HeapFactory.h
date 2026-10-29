#pragma once
#include <string>
#include <span>
#include <memory>
#include <string_view>
#include "BlockInfo.h"
#include "Heap.h"

class BestFitHeap;
class FirstFitHeap;
class NextFitHeap;
class BuddyHeap;
class SegregatedHeap;

enum class HeapType : int {
    FIRST_FIT = 0,
    NEXT_FIT,
    BEST_FIT, 
    BUDDY,
    SEGREGATED,
};

class HeapFactory {
public:
    static std::unique_ptr<Heap> createHeap(std::string_view type, int size, std::string_view config);
    static std::unique_ptr<Heap> createHeap(std::string_view type, int size, std::span<const BlockInfo> blocks, std::string_view config);
};
