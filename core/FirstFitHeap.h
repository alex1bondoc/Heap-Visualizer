#include "Heap.h"

class FirstFitHeap : Heap {
public:
    FirstFitHeap(int size);
    FirstFitHeap(int size, char *json);
    FirstFitHeap(const FirstFitHeap&) = delete;
    FirstFitHeap operator=(const FirstFitHeap) = delete;

    friend std::ostream& operator<<(std::ostream& os, const FirstFitHeap& heap);
    

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    friend std::string serialize(const FirstFitHeap& heap);
};
