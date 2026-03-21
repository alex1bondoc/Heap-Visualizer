#include "MemoryBlock.h"
#include <iostream>
#include <string>

class Heap {
private:

    Heap(int size); 
    Heap(int size, char *json);
    static Heap *instance; 
    MemoryBlock *head;
    int size;

public:
    Heap (const Heap&) = delete;
    Heap operator= (const Heap&) = delete;
    ~Heap();
    
    friend std::ostream& operator<<(std::ostream& os, const Heap& heap);

    static Heap *getInstance(int size);
    static Heap *getInstance(int size, char *json);

    MemoryBlock *getHead() { return head; }
    int getSize() { return size; }

    void setHead(MemoryBlock *head) { this->head = head; }
    void setSize(int size) { this->size = (size + 7) & ~7; }

    MemoryBlock *myMalloc(int size);
    MemoryBlock *myCalloc(int size);
    MemoryBlock *myRealloc(MemoryBlock *block, int size); 
    void myFree(MemoryBlock *block);

    friend std::string serialize(const Heap& heap);
};

