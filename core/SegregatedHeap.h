#pragma once
#include "Heap.h"
#include <string>

class SegregatedHeap : Heap {
private:
    static const int MIN_CLASS = 16;
    static const int MAX_CLASS_COUNT = 8;

    int classCount();
    int zoneSize();
    int zoneOf(int offset);
    int zoneFor(int size);
    int allocationSize(int size);
    std::string zoneLabel(int zone);
    int offsetOf(MemoryBlock *block);
    void splitInZone(MemoryBlock *block, int size);
    void mergeInZone(MemoryBlock *block);
public:
    SegregatedHeap(int size);
    SegregatedHeap(int size, char *json);
    SegregatedHeap(const SegregatedHeap&) = delete;
    SegregatedHeap operator=(const SegregatedHeap&) = delete;

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() override;
    std::string getType() override { return "SEGREGATED"; }
};
