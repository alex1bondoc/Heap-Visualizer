#include <iostream>

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
    Status status;
    int size;

    MemoryBlock *next;
    MemoryBlock *prev;

public: 
    MemoryBlock(int size, Status status, MemoryBlock *next = nullptr, MemoryBlock *prev = nullptr);
    
    friend std::ostream& operator<<(std::ostream& os, const MemoryBlock& memory_block);
    
    void setSize(int size) { this->size = size; }
    void setStatus(Status status) { this->status = status; }
    void setNext(MemoryBlock *next) { this->next = next; }
    void setPrev(MemoryBlock *prev) { this->prev = prev; }

    int getSize() { return size; }
    Status getStatus() { return status; }
    MemoryBlock *getNext() { return next; }
    MemoryBlock *getPrev() { return prev; }

    void removeCurrent(); 

    MemoryBlock *splitBlock(int size);    

    void joinNext(int size);
    void joinPrev(int size); 

    void mergeNext(); 
    void mergePrev(); 

    std::string serialize();
};
