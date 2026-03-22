#include "BestFitHeap.h"
#include <sstream>
#include <cstring>
#include <iostream>

BestFitHeap::BestFitHeap(int size) : Heap(size) {};
BestFitHeap::BestFitHeap(int size, char *json) : Heap(size, json) {};


std::ostream& operator<<(std::ostream& os, const BestFitHeap& heap) {
    os << "BestFitHeap: ";
    MemoryBlock *aux = heap.head;
    while (aux != nullptr) {
        os << *aux;
        aux = aux->getNext();
    }
    os << std::endl;
    return os;
}

MemoryBlock *BestFitHeap::myMalloc(int size) {
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

MemoryBlock *BestFitHeap::myCalloc(int size) {
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

MemoryBlock *BestFitHeap::myRealloc(MemoryBlock *block, int size) {
    size = (size + 7) & ~7;
    if (block == nullptr) {
        return myMalloc(size);
    }
    if (block->getStatus() == Status::FREE) {
        return nullptr;
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
                BestFitHeap::setHead(block);
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
                BestFitHeap::setHead(block);
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

void BestFitHeap::myFree(MemoryBlock *block) {
    if (block == nullptr || block->getStatus() == Status::FREE) {
        return;
    }
    block->setStatus(Status::FREE);
    block->mergeNext();
    block->mergePrev();
    if (block->getPrev() == nullptr) {
        BestFitHeap::setHead(block);
    }
}

std::string serialize(const BestFitHeap& heap) {
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
