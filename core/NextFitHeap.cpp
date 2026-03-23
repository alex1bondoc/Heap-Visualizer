#include "NextFitHeap.h"
#include "sstream"
#include <iostream>

NextFitHeap::NextFitHeap(int size) : Heap(size), last_allocated(this->head) {};
NextFitHeap::NextFitHeap(int size, char *json) : Heap(size, json), last_allocated(this->head) {};


std::ostream& operator<<(std::ostream& os, const NextFitHeap& heap) {
    os << "NextFitHeap: ";
    MemoryBlock *aux = heap.head;
    while (aux != nullptr) {
        os << *aux;
        aux = aux->getNext();
    }
    os << std::endl;
    return os;
}

MemoryBlock *NextFitHeap::myMalloc(int size) {
    size = (size + 7) & ~7;
    if (size == 0) {
        return nullptr;
    }
    MemoryBlock *aux = this->last_allocated;
    std::cout << *aux << std::endl;
    MemoryBlock *best_fit = nullptr;
    while (aux != nullptr && best_fit == nullptr) {
        if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
            best_fit = aux;
        }
        aux = aux->getNext();
    }
    if (best_fit != nullptr) {
        best_fit->splitBlock(size);
        this->last_allocated = best_fit;
        return best_fit;
    }
    aux = this->getHead();
    while(aux != nullptr && aux != last_allocated && best_fit == nullptr) {
        if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
            best_fit = aux;
        }
        aux = aux->getNext();
    }
    if (best_fit != nullptr) {
        best_fit->splitBlock(size);
        this->last_allocated = best_fit;
        return best_fit;
    }
    return nullptr;
}

MemoryBlock *NextFitHeap::myCalloc(int size) {
    size = (size + 7) & ~7;
    if (size == 0) {
        return nullptr;
    }
    MemoryBlock *aux = this->last_allocated;
    MemoryBlock *best_fit = nullptr;
    while (aux != nullptr && best_fit == nullptr) {
        if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
            best_fit = aux;
        }
        aux = aux->getNext();
    }
    if (best_fit != nullptr) {
        best_fit->splitBlock(size);
        last_allocated = best_fit;
        return best_fit;
    }
    aux = this->getHead();
    while(aux != nullptr && aux != last_allocated && best_fit == nullptr) {
        if (aux->getSize() >= size && aux->getStatus() == Status::FREE) {
            best_fit = aux;
        }
        aux = aux->getNext();
    }
    if (best_fit != nullptr) {
        best_fit->splitBlock(size);
        last_allocated = best_fit;
        return best_fit;
    }
    return nullptr;
}
MemoryBlock *NextFitHeap::myRealloc(MemoryBlock *block, int size) {
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
        last_allocated = block;
        return block->splitBlock(size);
    }
    else{
        MemoryBlock *next = block->getNext();
        MemoryBlock *prev = block->getPrev();
        if (next != nullptr && next->getStatus() == Status::FREE && block->getSize() + next->getSize() >= size) {
            block->joinNext(size - block->getSize());
            last_allocated = block;
            return block;
        }
        else if(prev != nullptr && prev->getStatus() == Status::FREE && block->getSize() + prev->getSize() >= size) {
            if (block->getSize() + prev->getSize() == size) {
                NextFitHeap::setHead(block);
            }
            block->joinPrev(size - block->getSize());
            last_allocated = block;
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
                NextFitHeap::setHead(block);
            }
            last_allocated = block;
            return block;
        }
        else{
            MemoryBlock *new_block = myMalloc(size);
            if (new_block != nullptr) {
                myFree(block);
            }
            else return block;
            last_allocated = new_block;
            return new_block;
        }
    }
    return nullptr;
}

void NextFitHeap::myFree(MemoryBlock *block) {
    if (block == nullptr || block->getStatus() == Status::FREE) {
        return;
    }
    block->setStatus(Status::FREE);
    if (block == last_allocated) {
        last_allocated = block->getPrev();
    }
    block->mergeNext();
    block->mergePrev();
    if (block->getPrev() == nullptr) {
        NextFitHeap::setHead(block);
    }
}

std::string serialize(const NextFitHeap& heap) {
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
