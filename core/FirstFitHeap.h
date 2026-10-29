#pragma once
#include <ostream>
#include <string>
#include "Heap.h"

class FirstFitHeap : public Heap {
public:
    FirstFitHeap(int size);
    FirstFitHeap(int size, char *json);

    friend std::ostream& operator<<(std::ostream& os, const FirstFitHeap& heap);
    

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() const override;
    std::string getType() const override { return "FIRST_FIT"; }
};
