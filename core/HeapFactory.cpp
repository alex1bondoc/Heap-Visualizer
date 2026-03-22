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
    return HeapType::FIRST_FIT;
}

Heap *HeapFactory::createHeap(std::string type, int size) {
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
        default:
            return nullptr;
            break;
    }
}
static BestFitHeap *createBestFitHeap(int size) {
    return new BestFitHeap(size);
}
static FirstFitHeap *createFirstFitHeap(int size) {
    return new FirstFitHeap(size);
}
static NextFitHeap *createNextFitHeap(int size) {
    return new NextFitHeap(size);
}
