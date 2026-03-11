#include "MemoryBlock.h"

class Heap {
private:

    Heap(int size); 

    static Heap *instance; 
    MemoryBlock *head;
    int size;

public:
    Heap (const Heap&) = delete;
    Heap operator= (const Heap&) = delete;
    ~Heap();
    
    static Heap *getInstance(int size);

    MemoryBlock *getHead() { return head; }
    int getSize() { return size; }

    void setHead(MemoryBlock *head) { this->head = head; }
    void setSize(int size) { this->size = (size + 7) & ~7; }

    MemoryBlock *myMalloc(int size);
    MemoryBlock *myCalloc(int size);
    MemoryBlock *myRealloc(MemoryBlock *block, int size); 
    void myFree(MemoryBlock *block);
};

