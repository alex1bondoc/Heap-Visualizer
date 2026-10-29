#pragma once
#include <ostream>
#include <string>

enum class Status : int {
    FREE = 0,
    ALLOC,
};

inline std::ostream& operator<<(std::ostream& os, const Status& status) {
    if (status == Status::FREE) {
        os << "FREE";
    }
    else {
        os << "ALLOC";
    }
    return os;
}

class MemoryBlock {
private:
    int size;
    Status status;

    MemoryBlock *next;
    MemoryBlock *prev;

    // Used only by BuddyHeap: the other half of the split that created this block,
    // and (for right halves) the buddy the parent block had before that split.
    MemoryBlock *buddy = nullptr;
    MemoryBlock *parentBuddy = nullptr;

public: 
    MemoryBlock(int size, Status status, MemoryBlock *next = nullptr, MemoryBlock *prev = nullptr);
    
    friend std::ostream& operator<<(std::ostream& os, const MemoryBlock& memory_block);
    
    void setSize(int size) { this->size = size; }
    void setStatus(Status status) { this->status = status; }
    void setNext(MemoryBlock *next) { this->next = next; }
    void setPrev(MemoryBlock *prev) { this->prev = prev; }
    void setBuddy(MemoryBlock *buddy) { this->buddy = buddy; }
    void setParentBuddy(MemoryBlock *parentBuddy) { this->parentBuddy = parentBuddy; }

    int getSize() { return size; }
    Status getStatus() { return status; }
    MemoryBlock *getNext() { return next; }
    MemoryBlock *getPrev() { return prev; }
    MemoryBlock *getBuddy() { return buddy; }
    MemoryBlock *getParentBuddy() { return parentBuddy; }

    void removeCurrent(); 

    MemoryBlock *splitBlock(int size);    

    void joinNext(int size);
    void joinPrev(int size); 

    void mergeNext(); 
    void mergePrev(); 

    std::string serialize();
};
