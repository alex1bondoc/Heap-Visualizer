#pragma once
#include <ostream>
#include <string>
#include "MemoryBlock.h"

class Heap{
protected:
    int size;
    MemoryBlock *head;
public:
    Heap(int size);
    Heap(int size, char *json);
    Heap (const Heap&) = delete;
    Heap& operator=(const Heap&) = delete;
    virtual ~Heap();

    friend std::ostream& operator<<(std::ostream& os, const Heap& heap);

    MemoryBlock *getHead() { return head; }
    int getSize() {return size;}

    void setHead(MemoryBlock *head) { this->head = head; }
    void setSize(int size) { this->size = (size + 7) & ~7; }
    
    virtual MemoryBlock *myMalloc(int size);
    virtual MemoryBlock *myCalloc(int size);
    virtual MemoryBlock *myRealloc(MemoryBlock *block, int size);
    virtual void myFree(MemoryBlock *block);

    virtual std::string serialize();
    virtual std::string getType() { return "HEAP"; }
    virtual std::string getConfig() { return ""; }
};
