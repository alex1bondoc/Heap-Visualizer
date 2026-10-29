#pragma once
#include <ostream>
#include <string>
#include "Heap.h"

class BestFitHeap : public Heap {
public:
    BestFitHeap(int size); 
    BestFitHeap(int size, char *json);
    
    friend std::ostream& operator<<(std::ostream& os, const BestFitHeap& heap);

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() const override;
    std::string getType() const override { return "BEST_FIT"; }
};

