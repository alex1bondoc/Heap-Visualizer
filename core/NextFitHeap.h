#pragma once
#include <ostream>
#include <string>
#include <span>
#include <string_view>
#include "BlockInfo.h"
#include "Heap.h"

class NextFitHeap : public Heap {
private:
    MemoryBlock *last_allocated = nullptr;
public:
    NextFitHeap(int size);
    NextFitHeap(int size, std::span<const BlockInfo> blocks);

    friend std::ostream& operator<<(std::ostream& os, const NextFitHeap& heap);
    

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() const override;
    std::string_view getType() const override { return "NEXT_FIT"; }
};
