#include <iostream>
#include "Heap.h"
#include <string>

class BestFitHeap : Heap{
private:

    BestFitHeap(int size); 
    BestFitHeap(int size, char *json);
    static BestFitHeap *instance; 

public:
    BestFitHeap (const BestFitHeap&) = delete;
    BestFitHeap operator= (const BestFitHeap&) = delete;
    ~BestFitHeap();
    
    friend std::ostream& operator<<(std::ostream& os, const BestFitHeap& heap);

    static BestFitHeap *getInstance(int size);
    static BestFitHeap *getInstance(int size, char *json);

    MemoryBlock *getHead() { return head; }
    int getSize() { return size; }

    void setHead(MemoryBlock *head) { this->head = head; }
    void setSize(int size) { this->size = (size + 7) & ~7; }

    MemoryBlock *myMalloc(int size) override;
    MemoryBlock *myCalloc(int size) override;
    MemoryBlock *myRealloc(MemoryBlock *block, int size) override;
    void myFree(MemoryBlock *block) override;

    friend std::string serialize(const BestFitHeap& heap);
};

