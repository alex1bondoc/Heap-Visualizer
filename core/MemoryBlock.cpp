#include "MemoryBlock.h"
#include <iostream>
#include <sstream>
#include <string>

MemoryBlock::MemoryBlock(int size, Status status, MemoryBlock *next, MemoryBlock *prev) : size(size), status(status), next(next), prev(prev) {};

std::ostream& operator<<(std::ostream& os, const MemoryBlock& memory_block) {
    os << "[";
    if (memory_block.status == Status::FREE) {
        os << "FREE: ";
    }
    else os << "ALLOC: ";
    os << memory_block.size << "] ";
    return os;
}

void MemoryBlock::removeCurrent() {
    MemoryBlock *aux_next = this->getNext();
    MemoryBlock *aux_prev = this->getPrev();
    if (aux_prev != nullptr) {
        aux_prev->setNext(aux_next);
    }
    if (aux_next != nullptr) {
        aux_next->setPrev(aux_prev);
    }
    delete this;
}

MemoryBlock *MemoryBlock::splitBlock(int size) {
    if (this->getSize() - size == 0) {
        this->setStatus(Status::ALLOC);
        return this;
    }
    MemoryBlock *aux = new MemoryBlock(this->getSize() - size, Status::FREE);
    this->setSize(size);
    this->setStatus(Status::ALLOC);
    MemoryBlock *next = this->getNext();
    this->setNext(aux);
    aux->setPrev(this);
    if (next != nullptr) {
        next->setPrev(aux);
    }
    aux->setNext(next);
    aux->mergeNext();
    return this;
}

void MemoryBlock::joinNext(int size) { this->setSize(this->getSize() + size);
    this->getNext()->setSize(next->getSize() - size);
    if (this->getNext()->getSize() == 0) {
        this->getNext()->removeCurrent();
    }
}

void MemoryBlock::joinPrev(int size) {
    this->setSize(this->getSize() + size);
    this->getPrev()->setSize(prev->getSize() - size);
    if (this->getPrev()->getSize() == 0) {
        this->getPrev()->removeCurrent();
    }
}

void MemoryBlock::mergeNext() {
    if (this->getNext() == nullptr || this->getNext()->getStatus() != Status::FREE) {
        return;
    }
    this->setSize(this->getSize() + this->getNext()->getSize());
    this->getNext()->removeCurrent();
}

void MemoryBlock::mergePrev() {
    if (this->getPrev() == nullptr || this->getPrev()->getStatus() != Status::FREE) {
        return;
    }
    this->setSize(this->getSize() + this->getPrev()->getSize());
    this->getPrev()->removeCurrent();
}

std::string serialize(const MemoryBlock& memory_block) {
    std::ostringstream ss; 
    ss << "{" << "\"id\":\"" << &memory_block << "\",\"status\":\"" << memory_block.status << "\",\"size\":" << memory_block.size << "}";
    return ss.str();
}
