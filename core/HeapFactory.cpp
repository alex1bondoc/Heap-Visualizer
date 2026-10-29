#include "HeapFactory.h"
#include "BestFitHeap.h"
#include "FirstFitHeap.h"
#include "NextFitHeap.h"
#include "BuddyHeap.h"
#include "SegregatedHeap.h"
#include <string_view>

HeapType stringToHeapType(std::string_view type){
    if (type == "FIRST_FIT") {
        return HeapType::FIRST_FIT;
    }
    else if (type == "NEXT_FIT") {
        return HeapType::NEXT_FIT;
    }
    else if (type == "BEST_FIT") {
        return HeapType::BEST_FIT;
    }
    else if (type == "BUDDY") {
        return HeapType::BUDDY;
    }
    else if (type == "SEGREGATED") {
        return HeapType::SEGREGATED;
    }
    return HeapType::FIRST_FIT;
}

std::unique_ptr<Heap> HeapFactory::createHeap(std::string_view type, int size, std::string_view config) {
    HeapType t = stringToHeapType(type);

    switch (t) {
        case HeapType::FIRST_FIT:
            return std::make_unique<FirstFitHeap>(size);
        case HeapType::NEXT_FIT:
            return std::make_unique<NextFitHeap>(size);
        case HeapType::BEST_FIT:
            return std::make_unique<BestFitHeap>(size);
        case HeapType::BUDDY:
            return std::make_unique<BuddyHeap>(size);
        case HeapType::SEGREGATED:
            return std::make_unique<SegregatedHeap>(size, config);
        default:
            return nullptr;
    }
}
std::unique_ptr<Heap> HeapFactory::createHeap(std::string_view type, int size, std::span<const BlockInfo> blocks, std::string_view config) {
    HeapType t = stringToHeapType(type);

    switch (t) {
        case HeapType::FIRST_FIT:
            return std::make_unique<FirstFitHeap>(size, blocks);
        case HeapType::NEXT_FIT:
            return std::make_unique<NextFitHeap>(size, blocks);
        case HeapType::BEST_FIT:
            return std::make_unique<BestFitHeap>(size, blocks);
        case HeapType::BUDDY:
            return std::make_unique<BuddyHeap>(size, blocks);
        case HeapType::SEGREGATED:
            return std::make_unique<SegregatedHeap>(size, blocks, config);
        default:
            return nullptr;
    }
}