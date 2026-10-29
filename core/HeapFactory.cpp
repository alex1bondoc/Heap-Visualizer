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

Heap *HeapFactory::createHeap(std::string_view type, int size, std::string_view config) {
    HeapType t = stringToHeapType(type);

    switch (t) {
        case HeapType::FIRST_FIT:
            return HeapFactory::createFirstFitHeap(size);
            break;
        case HeapType::NEXT_FIT:
            return HeapFactory::createNextFitHeap(size);
            break;
        case HeapType::BEST_FIT:
            return HeapFactory::createBestFitHeap(size);
            break;
        case HeapType::BUDDY:
            return HeapFactory::createBuddyHeap(size);
            break;
        case HeapType::SEGREGATED:
            return HeapFactory::createSegregatedHeap(size, config);
            break;
        default:
            return nullptr;
            break;
    }
}
Heap *HeapFactory::createHeap(std::string_view type, int size, char* json, std::string_view config) {
    HeapType t = stringToHeapType(type);

    switch (t) {
        case HeapType::FIRST_FIT:
            return new FirstFitHeap(size, json);
        case HeapType::NEXT_FIT:
            return new NextFitHeap(size, json);
        case HeapType::BEST_FIT:
            return new BestFitHeap(size, json);
        case HeapType::BUDDY:
            return new BuddyHeap(size, json);
        case HeapType::SEGREGATED:
            return new SegregatedHeap(size, json, config);
        default:
            return nullptr;
    }
}
BestFitHeap *HeapFactory::createBestFitHeap(int size) {
    return new BestFitHeap(size);
}
FirstFitHeap *HeapFactory::createFirstFitHeap(int size) {
    return new FirstFitHeap(size);
}
NextFitHeap *HeapFactory::createNextFitHeap(int size) {
    return new NextFitHeap(size);
}
BuddyHeap *HeapFactory::createBuddyHeap(int size) {
    return new BuddyHeap(size);
}
SegregatedHeap *HeapFactory::createSegregatedHeap(int size, std::string_view config) {
    return new SegregatedHeap(size, config);
}
