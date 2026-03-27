#include "Heap.h"

class NextFitHeap : Heap {
private:
    MemoryBlock *last_allocated = nullptr;
public:
    NextFitHeap(int size);
    NextFitHeap(int size, char *json);
    NextFitHeap(const NextFitHeap&) = delete;
    NextFitHeap operator=(const NextFitHeap) = delete;

    friend std::ostream& operator<<(std::ostream& os, const NextFitHeap& heap);
    

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    std::string serialize() override;
};
