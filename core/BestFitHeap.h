#include <iostream>
#include "Heap.h"
#include <string>

class BestFitHeap : Heap {
public:
    BestFitHeap(int size); 
    BestFitHeap(int size, char *json);
    BestFitHeap (const BestFitHeap&) = delete;
    BestFitHeap operator= (const BestFitHeap&) = delete;
    
    friend std::ostream& operator<<(std::ostream& os, const BestFitHeap& heap);

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() override;
};

