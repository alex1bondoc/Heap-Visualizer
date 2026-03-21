#include "MemoryBlock.h"
#include <iostream>
#include <string>

class BestFitHeap{
private:

    BestFitHeap(int size); 
    BestFitHeap(int size, char *json);
    static BestFitHeap *instance; 
    MemoryBlock *head;
    int size;

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

    MemoryBlock *myMalloc(int size);
    MemoryBlock *myCalloc(int size);
    MemoryBlock *myRealloc(MemoryBlock *block, int size); 
    void myFree(MemoryBlock *block);

    friend std::string serialize(const BestFitHeap& heap);
};

