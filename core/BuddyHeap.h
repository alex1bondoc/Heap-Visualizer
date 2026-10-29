#pragma once
#include "BlockInfo.h"
#include "Heap.h"
#include <string>
#include <span>
#include <string_view>

class BuddyHeap : public Heap {
private:
    static const int MIN_BLOCK = 16;

    void createTopBlocks();
    void splitHalf(MemoryBlock *block);
    bool canMerge(MemoryBlock *block) const;
    MemoryBlock *mergeWithBuddy(MemoryBlock *block);
public:
    BuddyHeap(int size);
    BuddyHeap(int size, std::span<const BlockInfo> blocks);

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string_view getType() const override { return "BUDDY"; }
};
