#pragma once
#include <ostream>
#include <string>
#include "Heap.h"

class NextFitHeap : public Heap {
private:
    MemoryBlock *last_allocated = nullptr;
public:
    NextFitHeap(int size);
    NextFitHeap(int size, char *json);

    friend std::ostream& operator<<(std::ostream& os, const NextFitHeap& heap);
    

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() const override;
    std::string getType() const override { return "NEXT_FIT"; }
};
