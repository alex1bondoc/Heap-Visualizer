#include "Heap.h"
#include <iostream>

Heap::Heap(int size) : size(size), head(new MemoryBlock(size, Status::FREE)) {}; 
Heap *Heap::instance = nullptr;

Heap *Heap::getInstance(int size) {
    if (Heap::instance == nullptr) {
        return new Heap(size);
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
    os << "Heap:";
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
