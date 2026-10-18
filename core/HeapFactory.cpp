#include "HeapFactory.h"

HeapType stringToHeapType(std::string type){
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

Heap *HeapFactory::createHeap(std::string type, int size, std::string config) {
    HeapType t = stringToHeapType(type);

    switch (t) {
        case HeapType::FIRST_FIT:
            return (Heap *)HeapFactory::createFirstFitHeap(size);
            break;
        case HeapType::NEXT_FIT:
            return (Heap *)HeapFactory::createNextFitHeap(size);
            break;
        case HeapType::BEST_FIT:
            return (Heap *)HeapFactory::createBestFitHeap(size);
            break;
        case HeapType::BUDDY:
            return (Heap *)new BuddyHeap(size);
            break;
        case HeapType::SEGREGATED:
            return (Heap *)new SegregatedHeap(size, config);
            break;
        default:
            return nullptr;
            break;
    }
}
Heap *HeapFactory::createHeap(std::string type, int size, char *json, std::string config) {
    HeapType t = stringToHeapType(type);

    switch (t) {
        case HeapType::FIRST_FIT:
            return (Heap *)new FirstFitHeap(size, json);
        case HeapType::NEXT_FIT:
            return (Heap *)new NextFitHeap(size, json);
        case HeapType::BEST_FIT:
            return (Heap *)new BestFitHeap(size, json);
        case HeapType::BUDDY:
            return (Heap *)new BuddyHeap(size, json);
        case HeapType::SEGREGATED:
            return (Heap *)new SegregatedHeap(size, json, config);
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
