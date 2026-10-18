#pragma once
#include "Heap.h"
#include <string>

class BuddyHeap : Heap {
private:
    static const int MIN_BLOCK = 16;

    int offsetOf(MemoryBlock *block);
    MemoryBlock *getBuddy(MemoryBlock *block);
    void splitHalf(MemoryBlock *block);
    void mergeWithNext(MemoryBlock *block);
public:
    BuddyHeap(int size);
    BuddyHeap(int size, char *json);
    BuddyHeap(const BuddyHeap&) = delete;
    BuddyHeap operator=(const BuddyHeap&) = delete;

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string getType() override { return "BUDDY"; }
};
