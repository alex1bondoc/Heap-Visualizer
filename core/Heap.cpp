#include "Heap.h"
#include <sstream>
#include <cstring>
#include <iostream>

Heap::Heap(int size) : size(size), head(new MemoryBlock(size, Status::FREE)) {}; 
Heap::Heap(int size, char *json) : size(size), head(nullptr){
    char *p = strtok(json, "[]");
    char *p2 = strtok(p, ",");
    MemoryBlock *aux = nullptr;
    Status current_status;
    while (p2 != nullptr) {
        if (strstr(p2, "status") != nullptr){
            if (strstr(p2, "FREE") != nullptr) {
                current_status = Status::FREE;
            }
            else {
                current_status = Status::ALLOC;
            }
        }
        else if (strstr(p2, "size") != nullptr){
            int x = 0, power = 1;
            bool foundNumber = false;
            for (int i = strlen(p2) - 1; i >= 0; --i) {
                if (p2[i] == ' ' && foundNumber) {
                        break;
                }
                else if (p2[i] <= '9' && p2[i] >= '0') {
                    foundNumber = true;
                    x += power * (p2[i] - '0');
                    power *= 10;
                }
            }
            if (aux == nullptr) {
                this->head = new MemoryBlock(x, current_status);
                aux = head;
            }
            else {
                MemoryBlock *newMemoryBlock = new MemoryBlock(x, current_status, nullptr, aux);
                aux->setNext(newMemoryBlock);
                aux = aux->getNext();
            }
        }
        p2 = strtok(nullptr, ",");
    }
    
};

Heap *Heap::instance = nullptr;

Heap *Heap::getInstance(int size) {
    if (Heap::instance == nullptr) {
        return new Heap(size);
    }
    return Heap::instance;
}

Heap *Heap::getInstance(int size, char *json) {
    if (Heap::instance == nullptr) {
        return new Heap(size, json);
    }
    return Heap::instance;
}

Heap::~Heap() {
    while (head != nullptr) {
        MemoryBlock *aux = head->getNext();
        delete head;
        head = aux;
    }
}

std::ostream& operator<<(std::ostream& os, const Heap& heap) {
    os << "Heap: ";
    MemoryBlock *aux = heap.head;
    while (aux != nullptr) {
        os << *aux;
        aux = aux->getNext();
    }
    os << std::endl;
    return os;
}

MemoryBlock *Heap::myMalloc(int size) {
    size = (size + 7) & ~7;
    if (size == 0) {
        return nullptr;     
    }
    MemoryBlock *aux = this->getHead();
    MemoryBlock *best_fit = nullptr;
    while (aux != nullptr) {
        if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
            if (best_fit == nullptr || aux->getSize() < best_fit->getSize()) {
                best_fit = aux;
            }
        }
        aux = aux->getNext();
    }
    if (best_fit == nullptr) {
        return nullptr;
    }
    best_fit->splitBlock(size);
    return best_fit;
}

MemoryBlock *Heap::myCalloc(int size) {
    size = (size + 7) & ~7;
    if (size == 0) {
        return nullptr;
    }
    MemoryBlock *aux = this->getHead();
    MemoryBlock *best_fit = nullptr;
    while (aux != nullptr) {
        if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
            if (best_fit == nullptr || aux->getSize() < best_fit->getSize()) {
                best_fit = aux;
            }
        }
        aux = aux->getNext();
    }
    if (best_fit == nullptr) {
        return nullptr;
    }
    best_fit = best_fit->splitBlock(size);
    return best_fit;
}

MemoryBlock *Heap::myRealloc(MemoryBlock *block, int size) {
    size = (size + 7) & ~7;
    if (block == nullptr) {
        return myMalloc(size);
    }
    if (size == 0) {
        myFree(block);
        return nullptr;
    }
    if (size <= block->getSize()) {
        return block->splitBlock(size);
    }
    else{
        MemoryBlock *next = block->getNext();
        MemoryBlock *prev = block->getPrev();
        if (next != nullptr && next->getStatus() == Status::FREE && block->getSize() + next->getSize() >= size) {
            block->joinNext(size - block->getSize());
            return block;
        }
        else if(prev != nullptr && prev->getStatus() == Status::FREE && block->getSize() + prev->getSize() >= size) {
            if (block->getSize() + prev->getSize() == size) {
                Heap::setHead(block);
            }
            block->joinPrev(size - block->getSize());
            return block;
        }
        else if(prev != nullptr && next != nullptr && prev->getStatus() == Status::FREE && next->getStatus() == Status::FREE && block->getSize() + next->getSize() + prev->getSize() >= size) {
            if (prev->getSize() < next->getSize()) {
                block->joinNext(next->getSize());
                block->joinPrev(size - block->getSize());
            }
            else {
                block->joinPrev(prev->getSize());
                block->joinNext(size - block->getSize());
            }
            if (block->getPrev() == nullptr) {
                Heap::setHead(block);
            }
            return block;
        }
        else{
            MemoryBlock *new_block = myMalloc(size);
            if (new_block != nullptr) {
                myFree(block);
            }
            else return block;
            return new_block;
        }
    }
    return nullptr;
}

void Heap::myFree(MemoryBlock *block) {
    if (block == nullptr || block->getStatus() == Status::FREE) {
        return;
    }
    block->setStatus(Status::FREE);
    block->mergeNext();
    block->mergePrev();
    if (block->getPrev() == nullptr) {
        Heap::setHead(block);
    }
}

std::string serialize(const Heap& heap) {
    std::ostringstream ss;
    MemoryBlock *aux = heap.head;
    ss << "[";
    while (aux != nullptr) {
        ss << serialize(*aux);
        aux = aux->getNext();
        if (aux != nullptr) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
