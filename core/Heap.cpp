#include "Heap.h"
#include <iostream>
#include <sstream>

Heap::Heap(int size) : size(size), head(new MemoryBlock(size, Status::FREE)) {};
Heap::Heap(int size, std::span<const BlockInfo> blocks) : size(size), head(nullptr){
    MemoryBlock *last = nullptr;
    for (const BlockInfo &block_info : blocks) {
        MemoryBlock *block = new MemoryBlock(block_info.size, block_info.status, nullptr, last);
        if (last == nullptr) {
            this->head = block;
        }
        else {
            last->setNext(block);
        }
        last = block;
    }
}

Heap::~Heap() {
    MemoryBlock *aux = head;
    while (head != nullptr) {
        aux = head->getNext();
        delete head;
        head = aux;
    }
}

MemoryBlock *Heap::myMalloc(int size) {
    return nullptr;
}
MemoryBlock *Heap::myCalloc(int size) {
    return nullptr;
}
MemoryBlock *Heap::myRealloc(MemoryBlock *block, int size) {
    return nullptr;
}
void Heap::myFree(MemoryBlock *block) {
}

std::ostream& operator<<(std::ostream& os, const Heap& heap) {
    std::cout << "Heap " << std::endl;
    MemoryBlock *aux = heap.head;
    while (aux != nullptr) {
        os << *aux;
        aux = aux->getNext();
    }
    os << std::endl;
    return os;
}

std::string Heap::serialize() const {
    std::ostringstream ss;
    MemoryBlock *aux = this->head;
    ss << "[";
    while (aux != nullptr) {
        ss << aux->serialize();
        aux = aux->getNext();
        if (aux != nullptr) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
